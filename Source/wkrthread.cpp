//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *
 *  $Revision: 1.2 $
 *
 *  $Log: wkrthread.cpp $
 *  Revision 1.2  2005/06/15 02:38:43  Administrator
 *  Added all internal documentation
 *
 *  Revision 1.1  2005/06/14 05:45:26  Administrator
 *  Initial revision
 *
 *
*/
//---------------------------------------------------------------------------

#pragma hdrstop

#include "wkrthread.h"
#include "mainfrm.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall Unit1::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

/*
 * Purpose : CTOR - prepares inital thread object.  Connects to LogThread (CS)
 *            and l. Creates Namne based on DSN and current count.
 *            Also prepares ODBC initialisation.
 * Accepts : Connection Data , LogEvent Pointer and shared
 *           Critical Section from Log
 * Returns : CTOR'd Object
*/
__fastcall wkrThread::wkrThread( connData * cd , TNotifyLog l ,
                                TCriticalSection * cs ) : TWaitThread ( )
{
    // DONE: Build DB thread to connect and run SQL
    CS = cs ;
    LogEvent  = l ;
    WaitInterval = cd->Timeout ;
    FreeOnTerminate = false ;
    FTableList = new TStringList ;

    FConnectionString  = cd->DSN ;

    FThreadNum = cd->RunCount + 1 ;


    ThreadName  = cd->DSN.SubString ( cd->DSN.Pos("=") + 1 ,
                                      cd->DSN.Pos(";")-1 )
                                      + "|" + IntToStr ( FThreadNum ) ;

    ODBCLibrary ( ); // set up the function calls

    /* Set up the Handles */
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

}
//---------------------------------------------------------------------------

/*
 * Purpose : DTOR - clean up and disconnect
 * Accepts : Nil
 * Returns : Nil
*/
__fastcall wkrThread::~wkrThread( )
{
    //DONE: Add your source code here
    delete FTableList ;

    retcode = SQLFreeHandle ( SQL_HANDLE_STMT , hstmt ) ;
    retcode = SQLDisconnect ( hdbc ) ;
    retcode = SQLFreeHandle ( SQL_HANDLE_DBC  , hdbc ) ;
    retcode = SQLFreeHandle ( SQL_HANDLE_ENV  , henv ) ;

    FreeLibrary ( hLib ) ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Overridden Execute Method
 * Accepts : Nil
 * Returns : Nil
*/
void __fastcall wkrThread::Execute()
{
    //---- Place thread code here ----

    retcode = MakeConnection ( ) ;

    if ( retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO )
    {
        threadLogEvent ( "---> Connection Failed" ) ;
    }
    else // If the connection succeeded.
    {
        retcode = ListTables ( ) ;

        while ( ! Terminated )
        {
            runSQL ( ) ;
        } // while
    } // if else
}
//---------------------------------------------------------------------------

/*
 * Purpose : Overriding Resume
 * Accepts : Nil
 * Returns : Nil
*/
void __fastcall wkrThread::Resume ( )
{
    Inherited::Resume ( ) ;

    if ( FOnResume )
        FOnResume ( this ) ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : ODBC Connection - assumes correct Connection string
 *            is passed into this thread.
 * Accepts : Nil
 * Returns : Nil
*/
SQLRETURN __fastcall wkrThread::MakeConnection( )
{
    //DONE: Add your source code here
    // Try the connection
    char szConnStrIn[150] = "" ;
    char szConnStrOut[150] = "" ;
    SQLSMALLINT cbConnStrOut ;

    lstrcpy(szConnStrIn , FConnectionString.c_str( ) ) ;

    threadLogEvent ( "---> Attempting connection to "
                      + FConnectionString.SubString ( FConnectionString.Pos( "=" ) + 1
                      , FConnectionString.Pos( ";" ) - FConnectionString.Pos( "=" ) - 1 ) ) ;

    do // potential dead loop here
    {
        retcode = SQLBrowseConnect( hdbc, szConnStrIn, SQL_NTS,
        szConnStrOut, sizeof(szConnStrOut), &cbConnStrOut);
    } while ( retcode == SQL_NEED_DATA) ;

    if ( retcode == SQL_ERROR )
    {
        threadLogEvent ( ">>-- Browse Connect failed - trying connnect--<<" ) ;

        int start = 1 , stop = 1 , tmp ;
        AnsiString a ;

        start = FConnectionString.Pos("DSN=") + 4 ;
        stop  = start ;
        while ( FConnectionString[stop]!=';')stop++ ;
        a = FConnectionString.SubString ( start , stop - start ) ;
        int szSvrName = a.Length ( ) ;
        SQLCHAR *ServerName = new SQLCHAR [ szSvrName ];
        strcpy ( ServerName , a.c_str ( ) );

        start = FConnectionString.Pos("UID=") + 4 ;
        stop  = start ;
        while ( FConnectionString[stop]!=';')stop++ ;
        a = FConnectionString.SubString ( start , stop - start) ;
        int szUID = a.Length ( ) ;
        SQLCHAR *UserName = new SQLCHAR [ szUID ];
        strcpy ( UserName , a.c_str (  ) );


        start = FConnectionString.Pos("PWD=") + 4 ;
        stop  = FConnectionString.Length ( ) ;
        a = FConnectionString.SubString ( start , stop - start + 1 ) ;
        int szPWD = a.Length ( ) ;
        SQLCHAR *Authentication = new SQLCHAR [ szPWD ];
        strcpy ( Authentication , a.c_str (  ) );

        retcode = SQLConnect(hdbc,
           ServerName     , szSvrName ,
           UserName       , szUID  ,
           Authentication , szPWD );

        delete[] ServerName ;
        delete[] UserName ;
        delete[] Authentication ;

    }

    return retcode ;

}
//---------------------------------------------------------------------------

/*
 * Purpose : Lists the tables in the DSN
 * Accepts : Nil
 * Returns : Nil
*/
SQLRETURN __fastcall wkrThread::ListTables()
{
    //DONE: Add your source code here
    threadLogEvent ( "---> Connection Succeeded - Listing Tables" ) ;

    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt ) ;
    retcode = SQLTables( hstmt, NULL , 0 , NULL , 0 , NULL , 0 , NULL , 0 ) ;

    // Bind Columns
    /*
        TABLE_CAT   	1 	Varchar
        TABLE_SCHEM 	2 	Varchar
        TABLE_NAME  	3 	Varchar
        TABLE_TYPE  	4 	Varchar
    */
    const SQLINTEGER STR_LEN = 128 ;
    SQLCHAR  szTableName [ STR_LEN + 1 ] , szColumnName  [ STR_LEN + 1 ] ;
    SQLCHAR  szTABLE_CAT [ STR_LEN + 1 ] , szTABLE_SCHEM [ STR_LEN + 1 ] ;
    SQLLEN cbTableName , cbColumnName , cbTABLE_CAT , cbTABLE_SCHEM ;

    retcode = SQLBindCol(hstmt , 1 , SQL_C_CHAR   , szTABLE_CAT  , STR_LEN , &cbTABLE_CAT  ) ;
    retcode = SQLBindCol(hstmt , 2 , SQL_C_CHAR   , szTABLE_SCHEM  , STR_LEN , &cbTABLE_SCHEM  ) ;

    retcode = SQLBindCol(hstmt , 3 , SQL_C_CHAR   , szTableName  , STR_LEN , &cbTableName  ) ;
    retcode = SQLBindCol(hstmt , 4 , SQL_C_CHAR   , szColumnName , STR_LEN , &cbColumnName ) ;

    /* Call SQLFETCH until it returns SQL_NO_DATA */
    /* If SQL_NO_DATA is returned.                             */
    while( retcode != SQL_NO_DATA )
    {
        retcode = SQLFetch(hstmt);

        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
        {
            // Only want user tables, not system and related ones. */
            if ( strcmp ( ( char * ) szTABLE_SCHEM , "TABLE")!=0) continue ;

            t = ( char * ) szTableName ;
            FTableList->Add ( t ) ;  //problem?  race condition?
            threadLogEvent  ( t ) ;
        } // if
    } // while

   SQLCloseCursor ( hstmt  ) ;
   SQLEndTran ( SQL_HANDLE_DBC , hdbc , SQL_COMMIT );

   return retcode  ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Function to select the table and run the SQL statment
 * Accepts : Nil
 * Returns : Nil
*/
void __fastcall wkrThread::runSQL()
{
    //DONE: Add your source code here
    SQLCHAR szTableName[129] ;
    SQLINTEGER cbTableSize ;

    AnsiString tbl =  FTableList->Strings [ ( random ( FTableList->Count ) - 1 + FThreadNum ) % FTableList->Count ] ;
    AnsiString sqlStmt = "SELECT COUNT(*) FROM " + tbl ;

    retcode = SQLPrepare( hstmt , sqlStmt.c_str ( ) , sqlStmt.Length ( ) ) ;
    retcode = SQLExecute( hstmt ) ;
    retcode = SQLBindCol( hstmt , 1 , SQL_CHAR , szTableName  , 128 , &cbTableSize  ) ;

    while(retcode != SQL_NO_DATA && retcode != SQL_ERROR)
    {
        retcode = SQLFetch(hstmt);
        AnsiString t = ( char * ) szTableName ;
        if (retcode != SQL_ERROR )
            threadLogEvent ( "---> SQL Result = " + tbl + " SQL Error" ) ;

        threadLogEvent ( "---> SQL Result = " + tbl + " " + t ) ;
    }

    SQLCloseCursor ( hstmt  ) ;
    SQLEndTran ( SQL_HANDLE_DBC , hdbc , SQL_COMMIT );
}
//---------------------------------------------------------------------------

/*
 * Purpose :  ODBC API Libray Functions
 *            - This function maps the ODBC32.DLL calls to pointers to
 *              functions.  It is an alternative to USELIB and required
 *              as the Library supplied causes "Unresolved External"
 *              compile time errors.
 *            - ODBC is used in preference to other built in types such as BDE and
 *              ADO as there is more control over the actual transactions and ADO
 *              was giving Runtime "Not licenced to use this feature" errors.
 * Accepts : Nil
 * Returns : Nil
*/
void __fastcall wkrThread::ODBCLibrary()
{
    //DONE: Add your source code here
    hLib = LoadLibrary ( "ODBC32.DLL" ) ;
    
    SQLAllocHandle  =
    (   SQLRETURN ( __stdcall * )
        ( SQLSMALLINT ,   SQLHANDLE   ,   SQLHANDLE	* )
    )  GetProcAddress ( hLib , "SQLAllocHandle" ) ;


     SQLBindCol =
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT	 ,  SQLUSMALLINT ,  SQLSMALLINT	 ,
	      SQLPOINTER	 ,  SQLLEN       ,  SQLLEN	    * )
     )  GetProcAddress ( hLib , "SQLBindCol" ) ;

     SQLBindParameter =
     (  SQLRETURN ( __stdcall * )(
	    SQLHSTMT	 ,  SQLUSMALLINT ,  SQLSMALLINT	 ,
       	SQLSMALLINT	 ,  SQLSMALLINT	 ,  SQLUINTEGER	 ,
	    SQLSMALLINT	 ,  SQLPOINTER	 ,   SQLINTEGER	 ,
	    SQLINTEGER	* )
     )  GetProcAddress ( hLib , "SQLBindParameter" ) ;

     SQLBrowseConnect =
     (  SQLRETURN ( __stdcall * )(
	    SQLHDBC		  , SQLCHAR	    * ,  SQLSMALLINT	  ,
	    SQLCHAR	    * , SQLSMALLINT	  , SQLSMALLINT	*  )
     )  GetProcAddress ( hLib , "SQLBrowseConnect" ) ;

    SQLCancel =
    (   SQLRETURN ( __stdcall * )
        (SQLHSTMT StatementHandle)
    )  GetProcAddress ( hLib , "SQLCancel" ) ;

    SQLCloseCursor =
    (   SQLRETURN ( __stdcall * )
        ( SQLHSTMT StatementHandle )
    )  GetProcAddress ( hLib , "SQLCloseCursor" ) ;

    SQLColAttribute =
    (   SQLRETURN ( __stdcall *  )
        ( SQLHSTMT	   ,    SQLUSMALLINT   ,    SQLUSMALLINT   ,
	      SQLPOINTER	   ,    SQLSMALLINT	   ,    SQLSMALLINT	 * ,
	      SQLPOINTER      )
    )  GetProcAddress ( hLib , "SQLColAttribute" ) ;

    SQLColumns =
    (   SQLRETURN ( __stdcall * )
        ( SQLHSTMT        , SQLCHAR	    * , SQLSMALLINT	  ,
	      SQLCHAR	    * , SQLSMALLINT	  , SQLCHAR	    * ,
          SQLSMALLINT	  , SQLCHAR	    * , SQLSMALLINT	)
    )  GetProcAddress ( hLib , "SQLColumns" ) ;

     SQLConnect =
     (  SQLRETURN ( __stdcall * )
	    ( SQLHDBC		  , SQLCHAR	    * , SQLSMALLINT	  ,
	      SQLCHAR	    * , SQLSMALLINT	  , SQLCHAR	    * ,
	      SQLSMALLINT )
     )  GetProcAddress ( hLib , "SQLConnect" ) ;

     SQLCopyDesc =
     (  SQLRETURN ( __stdcall * )
	    ( SQLHDESC      , SQLHDESC )
     )  GetProcAddress ( hLib , "SQLCopyDesc" ) ;

     SQLDataSources =
     (  SQLRETURN ( __stdcall * )
        ( SQLHENV		   ,    SQLUSMALLINT   ,    SQLCHAR	     * ,
	      SQLSMALLINT	   ,    SQLSMALLINT	 * ,    SQLCHAR	     * ,
	      SQLSMALLINT	   ,    SQLSMALLINT	 *	)
     )  GetProcAddress ( hLib , "SQLDataSources" ) ;

     SQLDescribeCol =
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT	       ,    SQLUSMALLINT   ,    SQLCHAR	     * ,
      	  SQLSMALLINT	   ,    SQLSMALLINT	 * ,    SQLSMALLINT	 * ,
	      SQLULEN	     * ,    SQLSMALLINT	 * ,    SQLSMALLINT	 *	)
     )  GetProcAddress ( hLib , "SQLDescribeCol" ) ;

     SQLDisconnect =
     (  SQLRETURN ( __stdcall * )
        ( SQLHDBC ConnectionHandle )
     )  GetProcAddress ( hLib , "SQLDisconnect" ) ;

     SQLEndTran =
     (  SQLRETURN ( __stdcall * )
	    ( SQLSMALLINT	   ,    SQLHANDLE	   ,    SQLSMALLINT )
     )  GetProcAddress ( hLib , "SQLEndTran" ) ;

     SQLExecDirect =
     (   SQLRETURN ( __stdcall * )
        ( SQLHSTMT	   ,    SQLCHAR      * ,    SQLINTEGER )
     )  GetProcAddress ( hLib , "SQLExecDirect" ) ;

     SQLExecute =
     (  SQLRETURN ( __stdcall * )
        (SQLHSTMT StatementHandle)
     )  GetProcAddress ( hLib , "SQLExecute" ) ;

     SQLFetch =
     (  SQLRETURN ( __stdcall * )
        (SQLHSTMT StatementHandle)
     )  GetProcAddress ( hLib , "SQLFetch" ) ;

     SQLFetchScroll =
     (  SQLRETURN ( __stdcall * )
	    ( SQLHSTMT	   ,    SQLSMALLINT	   ,    SQLROWOFFSET )
     )  GetProcAddress ( hLib , "SQLFetchScroll" ) ;

     SQLFreeHandle =
     (  SQLRETURN ( __stdcall * )
        ( SQLSMALLINT	   ,    SQLHANDLE )
     )  GetProcAddress ( hLib , "SQLFreeHandle" ) ;

     SQLFreeStmt =
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT	   ,    SQLUSMALLINT )
     )  GetProcAddress ( hLib , "SQLFreeStmt" ) ;

     SQLGetConnectAttr =
     (  SQLRETURN ( __stdcall * )
        ( SQLHDBC		   ,    SQLINTEGER	   ,    SQLPOINTER	   ,
    	  SQLINTEGER	   , SQLINTEGER	*	)
     )  GetProcAddress ( hLib , "SQLGetConnectAttr" ) ;

     SQLGetCursorName =
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT	   ,    SQLCHAR	     * ,
	      SQLSMALLINT	   ,    SQLSMALLINT	 *	)
     )  GetProcAddress ( hLib , "SQLGetCursorName" ) ;

     SQLGetData =
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT	   ,    SQLUSMALLINT   ,    SQLSMALLINT	   ,
    	SQLPOINTER	   ,    SQLLEN		   ,    SQLLEN       *	)
     )  GetProcAddress ( hLib , "SQLGetData" ) ;

     SQLGetDescField =
     (  SQLRETURN ( __stdcall * )
        ( SQLHDESC	   ,    SQLSMALLINT	   ,    SQLSMALLINT	   ,
    	  SQLPOINTER	   ,    SQLINTEGER	   ,    SQLINTEGER	 *	)
     )  GetProcAddress ( hLib , "SQLGetDescField" ) ;

     SQLGetDescRec =
     (  SQLRETURN ( __stdcall * )
        ( SQLHDESC	   ,    SQLSMALLINT	   ,    SQLCHAR	     * ,
    	  SQLSMALLINT	   ,    SQLSMALLINT	 * ,    SQLSMALLINT	 * ,
	      SQLSMALLINT	 * ,    SQLLEN	     * ,    SQLSMALLINT	 * ,
	      SQLSMALLINT	 * ,    SQLSMALLINT	 *	)
     )  GetProcAddress ( hLib , "SQLGetDescRec" ) ;

     SQLGetDiagField =
     (  SQLRETURN ( __stdcall * )
        ( SQLSMALLINT	   ,    SQLHANDLE	   ,    SQLSMALLINT	   ,
    	  SQLSMALLINT	   ,    SQLPOINTER	   ,    SQLSMALLINT	   ,
	      SQLSMALLINT	 *	)
     )  GetProcAddress ( hLib , "SQLGetDiagField" ) ;

     SQLGetDiagRec =
     (  SQLRETURN ( __stdcall * )
        ( SQLSMALLINT	   ,    SQLHANDLE	   ,    SQLSMALLINT	   ,
          SQLCHAR	     * ,    SQLINTEGER	 * ,    SQLCHAR	     * ,
    	  SQLSMALLINT	   ,    SQLSMALLINT	 *	)
     )  GetProcAddress ( hLib , "SQLGetDiagRec" ) ;

     SQLGetEnvAttr =
     (  SQLRETURN ( __stdcall * )
        ( SQLHENV		   ,    SQLINTEGER	   ,    SQLPOINTER	   ,
    	SQLINTEGER	   ,    SQLINTEGER	 *	)
     )  GetProcAddress ( hLib , "SQLGetEnvAttr" ) ;

     SQLGetFunctions =
     (  SQLRETURN ( __stdcall * )
        ( SQLHDBC		   ,    SQLUSMALLINT   ,    SQLUSMALLINT *	)
     )  GetProcAddress ( hLib , "SQLGetFunctions" ) ;

     SQLGetInfo =
     (  SQLRETURN ( __stdcall * )
        ( SQLHDBC		   ,    SQLUSMALLINT   ,    SQLPOINTER	   ,
    	  SQLSMALLINT	   ,    SQLSMALLINT	 *  )
     )  GetProcAddress ( hLib , "SQLGetInfo" ) ;

     SQLGetStmtAttr =
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT		,   SQLINTEGER		,   SQLPOINTER		,
    	  SQLINTEGER		,   SQLINTEGER	 *	 )
     )  GetProcAddress ( hLib , "SQLGetStmtAttr" ) ;

     SQLGetTypeInfo =
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT		,   SQLSMALLINT )
     )  GetProcAddress ( hLib , "SQLGetTypeInfo" ) ;

     SQLNumResultCols =
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT		,   SQLSMALLINT	 *	)
     )  GetProcAddress ( hLib , "SQLNumResultCols" ) ;

     SQLParamData =
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT		,   SQLPOINTER	 *	 )
     )  GetProcAddress ( hLib , "SQLParamData" ) ;

     SQLPrepare =
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT	   ,    SQLCHAR	     * ,    SQLINTEGER		)
     )  GetProcAddress ( hLib , "SQLPrepare" ) ;

     SQLPutData =
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT	   ,    SQLPOINTER	   ,    SQLLEN )
     )  GetProcAddress ( hLib , "SQLPutData" ) ;

     SQLRowCount =
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT	   ,    SQLLEN	     *	)
     )  GetProcAddress ( hLib , "SQLRowCount" ) ;

     SQLSetConnectAttr =
     (  SQLRETURN ( __stdcall * )
        ( SQLHDBC		   ,    SQLINTEGER	   ,
    	  SQLPOINTER	   ,    SQLINTEGER		)
     )  GetProcAddress ( hLib , "SQLSetConnectAttr" ) ;

     SQLSetCursorName =
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT	   ,    SQLCHAR	     * ,    SQLSMALLINT		)
     )  GetProcAddress ( hLib , "SQLSetCursorName" ) ;

     SQLSetDescField =
     (  SQLRETURN ( __stdcall * )
        ( SQLHDESC	   ,    SQLSMALLINT	   ,    SQLSMALLINT	   ,
    	  SQLPOINTER	   ,    SQLINTEGER		)
     )  GetProcAddress ( hLib , "SQLSetDescField" ) ;

     SQLSetDescRec =
     (  SQLRETURN ( __stdcall * )
        ( SQLHDESC	   ,    SQLSMALLINT	   ,    SQLSMALLINT	   ,
    	  SQLSMALLINT	   ,    SQLLEN		   ,    SQLSMALLINT	   ,
    	  SQLSMALLINT	   ,    SQLPOINTER	   ,    SQLLEN	     * ,
    	  SQLLEN	     *	)
     )  GetProcAddress ( hLib , "SQLSetDescRec" ) ;

     SQLSetEnvAttr =
     (  SQLRETURN ( __stdcall * )
        ( SQLHENV   ,   SQLINTEGER	   ,    SQLPOINTER	   ,
    	  SQLINTEGER )
     )  GetProcAddress ( hLib , "SQLSetEnvAttr" ) ;

     SQLSetStmtAttr =
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT	   ,    SQLINTEGER	   ,
    	  SQLPOINTER	   ,    SQLINTEGER		)
     )  GetProcAddress ( hLib , "SQLSetStmtAttr" ) ;

     SQLSpecialColumns =
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT	   ,    SQLUSMALLINT   ,    SQLCHAR	     * ,
    	  SQLSMALLINT	   ,    SQLCHAR	     * ,    SQLSMALLINT	   ,
    	  SQLCHAR	     * ,    SQLSMALLINT	   ,    SQLUSMALLINT   ,
    	  SQLUSMALLINT	)
     )  GetProcAddress ( hLib , "SQLSpecialColumns" ) ;

     SQLStatistics=
     (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT	   ,    SQLCHAR	     * ,    SQLSMALLINT	   ,
    	  SQLCHAR	     * ,    SQLSMALLINT	   ,    SQLCHAR	     * ,
    	  SQLSMALLINT	   ,    SQLUSMALLINT   ,    SQLUSMALLINT	)
     )  GetProcAddress ( hLib , "SQLStatistics" ) ;

    SQLTables =
    (  SQLRETURN ( __stdcall * )
        ( SQLHSTMT	   ,    SQLCHAR	     * ,    SQLSMALLINT	   ,
    	  SQLCHAR	     * ,    SQLSMALLINT	   ,    SQLCHAR	     * ,
    	  SQLSMALLINT	   ,    SQLCHAR	     * ,    SQLSMALLINT		)
    )  GetProcAddress ( hLib , "SQLTables" ) ;
}
