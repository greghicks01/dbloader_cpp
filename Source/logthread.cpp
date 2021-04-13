//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: logthread.cpp $
 *  Revision 1.2  2005/06/15 02:38:43  Administrator
 *  Added all internal documentation
 *
 *  Revision 1.1  2005/06/14 05:45:26  Administrator
 *  Initial revision
 *
 *
*/
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "logthread.h" 
#include "mainfrm.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

/*
 * Purpose : CTOR - prepares the log for operation
 * Accepts : Nil
 * Returns : * TLogThread
*/
__fastcall TLogThread::TLogThread()
    : TThread(true)
{
    // Log File
    MessageQueue = new TStringList ;
    FCS = new TCriticalSection ;
    LogFile = FileOpen ( "ODBCLogs.txt" , fmOpenWrite | fmShareExclusive ) ;
    if ( LogFile <= 0 )
        LogFile =  FileCreate ( "ODBCLogs.txt" ) ;

    CS->Enter ( ) ;
    LogEvent ( ">>>--- Log Thread Ready ---<<<" ) ;
    CS->Leave ( ) ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : DTOR - clean up
 * Accepts : Nil
 * Returns : Nil
*/
__fastcall TLogThread::~TLogThread()
{
    
    CS->Enter ( ) ;
    LogEvent ( ">>>--- Log Thread Closed ---<<<" ) ;
    CS->Leave ( ) ;
    FileClose ( LogFile ) ;
    delete FCS ;
    delete MessageQueue ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Overridden Execute  - wakes to write the log
 * Accepts : Nil
 * Returns : Nil
*/
void __fastcall TLogThread::Execute()
{
    //---- Place thread code here ----
    while ( ! Terminated )
    {
        Suspend ( ) ;
        Synchronize ( syncLogEvent ) ;
        Sleep ( 1 ) ;
    }
}
//---------------------------------------------------------------------------

/*
 * Purpose : VCL synchronised Log write to screen and file
 * Accepts : Nil
 * Returns : Nil
*/
void __fastcall TLogThread::syncLogEvent( )
{
    CS->Enter (  ) ;
    while ( MessageQueue->Count > 0 )
    {
        frmMainFrm->OperationalLog->Lines->Add ( MessageQueue->Strings[0] ) ;
        FileWrite ( LogFile , (MessageQueue->Strings [ 0 ] ).c_str ( ) ,
                    MessageQueue->Strings [ 0 ] . Length ( ) ) ;

        FileWrite ( LogFile , "\r\n" , 2 ) ;

        MessageQueue->Delete ( 0 ) ;
        if ( frmMainFrm->OperationalLog->Lines->Count > 50 )
            frmMainFrm->OperationalLog->Lines->Delete( 0 ) ;
    }
    CS->Leave (  ) ;
}
//---------------------------------------------------------------------------

/*
 * Purpose :  TNotifyLog event handler
 * Accepts :  String message
 * Returns :  Nil
*/
void __fastcall TLogThread::LogEvent ( AnsiString s )
{
    //DONE: Add your source code here
    MessageQueue->Add ( s ) ;
    Sleep ( 1 ) ;
    Resume ( ) ;
}
