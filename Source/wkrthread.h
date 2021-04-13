//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: wkrthread.h $
 *  Revision 1.2  2005/06/15 02:38:04  Administrator
 *  Added all internal documentation
 *
 *  Revision 1.1  2005/06/14 05:41:48  Administrator
 *  Initial revision
 *
 *
*/
//--------------------------------------------------------------------------- 
/*

    This thread is an asynchronous worker created by the factory thread.
    Its mission is to provide a working load (albiet artificial) via the
    ODBC connection to the server under test.

    This is done by gettng a list of tables, and on each iteration, randomly
    selecting a new table to run the SQL statement on.

    Unfortunately, the ODBC API was inserted directly into this thread, making
    it a long read.  A future project will be to encapsulate ODBC into its own
    custom component to reduce this code.

*/

#ifndef wkrthreadH
#define wkrthreadH
//---------------------------------------------------------------------------
#include "waitthread.h"
#include "conndata.h" // conndata
#include <sql.h>
#include <sqlext.h>
#include <Classes.hpp>
//---------------------------------------------------------------------------
class wkrThread : public TWaitThread
{
    typedef Inherited TWaitThread ;

    typedef void __fastcall ( __closure * TNotifyLog ) ( AnsiString ) ;

    private:
        // SQL ODBC Connections
        HINSTANCE hLib ;
        SQLHENV   henv ;
        SQLHDBC   hdbc ;
        SQLHSTMT  hstmt ;
        SQLRETURN retcode ;

	  int FThreadNum ;

        AnsiString FConnectionString ;
        AnsiString t ; // temporary for tables

        // ODBC Functions Signatures
        SQLRETURN ( __stdcall * SQLAllocHandle)(
            SQLSMALLINT ,   SQLHANDLE   ,   SQLHANDLE	* ) ;

        SQLRETURN ( __stdcall * SQLBindCol)(
            SQLHSTMT	 ,  SQLUSMALLINT ,  SQLSMALLINT	 ,
            SQLPOINTER	 ,  SQLLEN       ,  SQLLEN	    * ) ;

        SQLRETURN ( __stdcall * SQLBindParameter)(
            SQLHSTMT	 , SQLUSMALLINT , SQLSMALLINT	 ,
            SQLSMALLINT	 , SQLSMALLINT	, SQLUINTEGER	 ,
            SQLSMALLINT	 , SQLPOINTER   , SQLINTEGER	 ,
            SQLINTEGER	* ) ;

        SQLRETURN ( __stdcall * SQLBrowseConnect)(
            SQLHDBC		  , SQLCHAR	    * , SQLSMALLINT	  ,
            SQLCHAR	    * , SQLSMALLINT	  , SQLSMALLINT	*  ) ;

        SQLRETURN ( __stdcall * SQLCancel)(SQLHSTMT StatementHandle)    ;

        SQLRETURN ( __stdcall * SQLCloseCursor)(SQLHSTMT StatementHandle)   ;

        SQLRETURN ( __stdcall * SQLColAttribute )(
            SQLHSTMT	   ,    SQLUSMALLINT   ,    SQLUSMALLINT   ,
            SQLPOINTER	   ,    SQLSMALLINT	   ,
            SQLSMALLINT	 * ,    SQLPOINTER      ) ;


        SQLRETURN ( __stdcall * SQLColumns )(
            SQLHSTMT	  , SQLCHAR	    * , SQLSMALLINT	  ,
            SQLCHAR	    * , SQLSMALLINT	  , SQLCHAR	    * ,
            SQLSMALLINT	  , SQLCHAR	    * , SQLSMALLINT   ) ;


        SQLRETURN ( __stdcall * SQLConnect )(
            SQLHDBC		  , SQLCHAR	    * , SQLSMALLINT	  ,
            SQLCHAR	    * , SQLSMALLINT	  , SQLCHAR	    * ,
            SQLSMALLINT	   ) ;

        SQLRETURN ( __stdcall * SQLCopyDesc) (
            SQLHDESC      , SQLHDESC       ) ;

        SQLRETURN ( __stdcall * SQLDataSources) (
            SQLHENV		   ,    SQLUSMALLINT   ,    SQLCHAR	     * ,
            SQLSMALLINT	   ,    SQLSMALLINT	 * ,    SQLCHAR	     * ,
            SQLSMALLINT	   ,    SQLSMALLINT	 *	);

        SQLRETURN ( __stdcall * SQLDescribeCol)(
            SQLHSTMT	   ,    SQLUSMALLINT   ,    SQLCHAR	     * ,
            SQLSMALLINT	   ,    SQLSMALLINT	 * ,    SQLSMALLINT	 * ,
            SQLULEN	     * ,    SQLSMALLINT	 * ,    SQLSMALLINT	 *	) ;

        SQLRETURN ( __stdcall * SQLDisconnect)(SQLHDBC ConnectionHandle) ;

        SQLRETURN ( __stdcall * SQLEndTran)(
            SQLSMALLINT	   ,    SQLHANDLE	   ,    SQLSMALLINT		) ;

        SQLRETURN ( __stdcall * SQLExecDirect)(
            SQLHSTMT	   ,    SQLCHAR      * ,    SQLINTEGER		) ;

        SQLRETURN ( __stdcall * SQLExecute)(SQLHSTMT StatementHandle) ;

        SQLRETURN ( __stdcall * SQLFetch)(SQLHSTMT StatementHandle) ;

        SQLRETURN ( __stdcall * SQLFetchScroll)(
            SQLHSTMT	   ,    SQLSMALLINT	   ,    SQLROWOFFSET	) ;

        SQLRETURN ( __stdcall * SQLFreeHandle)(
            SQLSMALLINT	   ,    SQLHANDLE		) ;

        SQLRETURN ( __stdcall * SQLFreeStmt)(
            SQLHSTMT	   ,    SQLUSMALLINT	) ;

        SQLRETURN ( __stdcall * SQLGetConnectAttr)(
            SQLHDBC		   ,    SQLINTEGER	   ,    SQLPOINTER	   ,
            SQLINTEGER	   ,    SQLINTEGER	*	) ;

        SQLRETURN ( __stdcall * SQLGetCursorName)(
            SQLHSTMT	   ,    SQLCHAR	     * ,
            SQLSMALLINT	   ,    SQLSMALLINT	 *	) ;

        SQLRETURN ( __stdcall * SQLGetData)(
            SQLHSTMT	   ,    SQLUSMALLINT   ,    SQLSMALLINT	   ,
            SQLPOINTER	   ,    SQLLEN		   ,    SQLLEN       *	);

        SQLRETURN ( __stdcall * SQLGetDescField)(
            SQLHDESC	   ,    SQLSMALLINT	   ,    SQLSMALLINT	   ,
            SQLPOINTER	   ,    SQLINTEGER	   ,    SQLINTEGER	 *	);

        SQLRETURN ( __stdcall * SQLGetDescRec)(
            SQLHDESC	   ,    SQLSMALLINT	   ,    SQLCHAR	     * ,
            SQLSMALLINT	   ,    SQLSMALLINT	 * ,    SQLSMALLINT	 * ,
            SQLSMALLINT	 * ,    SQLLEN	     * ,    SQLSMALLINT	 * ,
            SQLSMALLINT	 * ,    SQLSMALLINT	 *	) ;

        SQLRETURN ( __stdcall * SQLGetDiagField)(
            SQLSMALLINT	   ,    SQLHANDLE	   ,    SQLSMALLINT	   ,
            SQLSMALLINT	   ,    SQLPOINTER	   ,    SQLSMALLINT	   ,
            SQLSMALLINT	 *	);

        SQLRETURN ( __stdcall * SQLGetDiagRec)(
            SQLSMALLINT	   ,    SQLHANDLE	   ,    SQLSMALLINT	   ,
            SQLCHAR	     * ,    SQLINTEGER	 * ,    SQLCHAR	     * ,
            SQLSMALLINT	   ,    SQLSMALLINT	 *	) ;

        SQLRETURN ( __stdcall * SQLGetEnvAttr)(
            SQLHENV		   ,    SQLINTEGER	   ,    SQLPOINTER	   ,
            SQLINTEGER	   ,    SQLINTEGER	 *	) ;

        SQLRETURN ( __stdcall * SQLGetFunctions)(
            SQLHDBC		   ,    SQLUSMALLINT   ,    SQLUSMALLINT *	);

        SQLRETURN ( __stdcall * SQLGetInfo)(
            SQLHDBC		   ,    SQLUSMALLINT   ,    SQLPOINTER	   ,
            SQLSMALLINT	   ,    SQLSMALLINT	 *  ) ;

        SQLRETURN ( __stdcall * SQLGetStmtAttr)(
            SQLHSTMT		,   SQLINTEGER		,   SQLPOINTER		,
            SQLINTEGER		,   SQLINTEGER	 *	 ) ;

        SQLRETURN ( __stdcall * SQLGetTypeInfo)(
            SQLHSTMT		,   SQLSMALLINT		 ) ;

        SQLRETURN ( __stdcall * SQLNumResultCols)(
            SQLHSTMT		,   SQLSMALLINT	 *	) ;

        SQLRETURN ( __stdcall * SQLParamData)(
            SQLHSTMT		,   SQLPOINTER	 *	 ) ;

        SQLRETURN ( __stdcall * SQLPrepare)(
            SQLHSTMT	   ,    SQLCHAR	     * ,    SQLINTEGER );

        SQLRETURN ( __stdcall * SQLPutData)(
            SQLHSTMT	   ,    SQLPOINTER	   ,    SQLLEN );

        SQLRETURN ( __stdcall * SQLRowCount)(
            SQLHSTMT	   ,    SQLLEN	     *	) ;

        SQLRETURN ( __stdcall * SQLSetConnectAttr)(
            SQLHDBC		   ,    SQLINTEGER	   ,
            SQLPOINTER	   ,    SQLINTEGER		) ;

        SQLRETURN ( __stdcall * SQLSetCursorName)(
            SQLHSTMT	   ,    SQLCHAR	     * ,    SQLSMALLINT		);

        SQLRETURN ( __stdcall * SQLSetDescField)(
            SQLHDESC	   ,    SQLSMALLINT	   ,    SQLSMALLINT	   ,
            SQLPOINTER	   ,    SQLINTEGER		) ;

        SQLRETURN ( __stdcall * SQLSetDescRec)(
            SQLHDESC	   ,    SQLSMALLINT	   ,    SQLSMALLINT	   ,
            SQLSMALLINT	   ,    SQLLEN		   ,    SQLSMALLINT	   ,
            SQLSMALLINT	   ,    SQLPOINTER	   ,    SQLLEN	     * ,
            SQLLEN	     *	);

        SQLRETURN ( __stdcall * SQLSetEnvAttr)(
            SQLHENV		EnvironmentHandle,
            SQLINTEGER	   ,    SQLPOINTER	   ,    SQLINTEGER		);

        SQLRETURN ( __stdcall * SQLSetStmtAttr)(
            SQLHSTMT	   ,    SQLINTEGER	   ,
            SQLPOINTER	   ,    SQLINTEGER		) ;

        SQLRETURN ( __stdcall * SQLSpecialColumns)(
            SQLHSTMT	   ,    SQLUSMALLINT   ,    SQLCHAR	     * ,
            SQLSMALLINT	   ,    SQLCHAR	     * ,    SQLSMALLINT	   ,
            SQLCHAR	     * ,    SQLSMALLINT	   ,    SQLUSMALLINT   ,
            SQLUSMALLINT	) ;

        SQLRETURN ( __stdcall * SQLStatistics)(
            SQLHSTMT	   ,    SQLCHAR	     * ,    SQLSMALLINT	   ,
            SQLCHAR	     * ,    SQLSMALLINT	   ,    SQLCHAR	     * ,
            SQLSMALLINT	   ,    SQLUSMALLINT   ,    SQLUSMALLINT	) ;

        SQLRETURN ( __stdcall * SQLTables)(
            SQLHSTMT	   ,    SQLCHAR	     * ,    SQLSMALLINT	   ,
            SQLCHAR	     * ,    SQLSMALLINT	   ,    SQLCHAR	     * ,
            SQLSMALLINT	   ,    SQLCHAR	     * ,    SQLSMALLINT		) ;

        void __fastcall ODBCLibrary();

        AnsiString FThreadName   ;
        TStringList * FTableList ;

        // Event Notifiers
        TNotifyLog FFactoryLog ; // Logging back to main form
        TNotifyEvent FOnResume ; // When Resume is needed

       
        // properties
        void __fastcall SetThreadName ( AnsiString ) ;
        SQLRETURN __fastcall MakeConnection( );
        SQLRETURN __fastcall ListTables();
        void __fastcall runSQL();

        // VCL Synchronised methods
    protected:
        void __fastcall Execute ( ) ;

    public:
        __fastcall wkrThread  ( connData * c , TNotifyLog , TCriticalSection * ) ;
        __fastcall ~wkrThread ( ) ;

        void __fastcall Resume( );

        // Event management
        __property TNotifyLog   FactoryLog = { read = FFactoryLog , write = FFactoryLog   } ;
        __property TNotifyEvent   OnResume = { read = FOnResume   , write = FOnResume     } ;        
};
//---------------------------------------------------------------------------
#endif
