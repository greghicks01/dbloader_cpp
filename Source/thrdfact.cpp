//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: thrdfact.cpp $
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

#include "thrdfact.h"
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
 * Purpose : CTOR - Prepares factory thread
 * Accepts : DSN List with all connection data , interval timer ,
 *            shared CS with Log , and log address
 * Returns :
*/
__fastcall ThreadFactory::ThreadFactory( TStringList * DSNList , int launchTimer , TCriticalSection * cs , TNotifyLog l )
        : TWaitThread ( )
{
     FWorkerList  = new TThreadList ;
     FDSNConnList = new TStringList ;
     WaitInterval = launchTimer     ;

     CS = cs ;
     LogEvent      = l ;
     CSEntered = false ;
     ListLocked = false ;
     ThreadName = "Factory|1" ;

     FreeOnTerminate = false ;

     FDSNConnList->AddStrings ( DSNList ) ;
     threadLogEvent ( ">>-- Factory Started --<<" ) ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : DTOR - clean up
 * Accepts : Nil
 * Returns : Nil
*/
__fastcall ThreadFactory::~ThreadFactory ( )
{
    threadLogEvent(  ">>-- Factory Stopped Production --<<" ) ;

    delete FWorkerList ;
    delete FDSNConnList ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Completes a single worker thread creation
 * Accepts : Connection Data
 * Returns : false if creation throws an error
*/
bool ThreadFactory::workerThreadCreate( connData * c )
{
    //DONE: Add your source code here
    wkrThread * w = NULL ;
    try
    {
        w = new wkrThread ( c , LogEvent , CS ) ;
        if ( w )
        {
            c->RunCount++ ;
            FWorkerList->Add ( static_cast < void * > ( w ) ) ;
        }
        else
        {
            c->NewFail = true ;

            threadLogEvent( ">-- Worker Allocation Fail --<" ) ;
        }
    }
    catch(...)
    {
       c->NewFail = true ;

       threadLogEvent( ">-- Worker Allocation Fail --<" ) ;
    }
    return ( ! c->NewFail ) ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : overridden Resume
 * Accepts : Nil
 * Returns : Nil
*/
void __fastcall ThreadFactory::Resume ( )
{
    threadLogEvent( ">>-- Woke up --<<" ) ;
    Inherited::Resume ( ) ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Terminates the thread by flagging the Terminated to true
 *            Also must kill off the worker threads before completing
 *            its own termination
 * Accepts : Nil
 * Returns : Nil
*/
void __fastcall ThreadFactory::Terminate ( )
{
    // ensure locks are released first.
    if ( ListLocked )
        FWorkerList->UnlockList ( ) ;
    if ( CSEntered )
        CS->Leave ( ) ;
        
    Inherited::Terminate ( ) ;
    
    threadLogEvent( ">>>-- Received kill all threads signal --<<<" ) ;

    KillAllThreads ( ) ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Overridden Execute
 * Accepts : Nil
 * Returns : Nil
*/
void __fastcall ThreadFactory::Execute( )
{
    //DONE:---- Place thread code here ----

    threadLogEvent( ">>-- Creating initial workcrew --<<" ) ;

    // 1. Open one of each, try to connect failures here require terminate

    if ( ODBCWkrs ( ) ) // false means the initial threads failed
    {
        threadLogEvent(  ">>> Connection problem - Terminating all threads <<<" ) ;
        Terminate ( ) ;
        return ;
    }

    threadLogEvent( ">>-- Creating complete workcrew compliment --<<" ) ;
    while ( ! Terminated && ! ODBCWkrs ( ) ) ;

    threadLogEvent ( ">-- Starting Workcrew --<" ) ;

    TList * t = FWorkerList->LockList ( ) ;
    for ( int i =0 ; i < t->Count ; i++ )
    {
        wkrThread * w = static_cast < wkrThread * > ( t->Items [ i ] );
        w->Resume ( ) ;
    }
    FWorkerList->UnlockList ( ) ;

    WaitInterval = FTestTime ;
    WaitForInterval( ) ;

    KillAllThreads ( ) ;

}
//---------------------------------------------------------------------------

/*
 * Purpose : Creates a thread for each ODBC Connection selected
 * Accepts : Nil
 * Returns : False if there is a problem creating the worker
*/
bool ThreadFactory::ODBCWkrs ( )
{
    // Creates one of each, counting creation and flagging fail
    bool answer = false ;
    for ( int i = 0 ; i < FDSNConnList->Count ; i++ )
    {
        connData * c = static_cast < connData * > ( FDSNConnList->Objects [ i ] ) ;
        answer |= c->NewFail || c->ConFail ;
        if (( c->NewFail || c->ConFail ) || c->RunCount >= c->MaxCons ) continue ;

        if ( workerThreadCreate ( c ) )
            WaitForInterval ( ) ;
        answer |= ! c->RunCount <= c->MaxCons ;
    } // for

    return ( ! answer ) ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Terminates all the workers this process created
 * Accepts : Nil
 * Returns : Nil
*/
void __fastcall ThreadFactory::KillAllThreads()
{
    //DONE: Add your source code here

    threadLogEvent ( ">>--- Kill all Worker Threads ---<<" ) ;

    TList * t = FWorkerList->LockList() ; // race condition when al ready locked
    for ( int i = 0 ; i < t->Count ; i++ )
    {
        wkrThread * w = static_cast < wkrThread * > ( t->Items [ i ] ) ;
        w->Terminate ( ) ;
        w->Finish ( ) ;
        w->Resume ( ) ;
    }
    for ( int i = 0 ; i < t->Count ; i++ )
    {
        wkrThread * w = static_cast < wkrThread * > ( t->Items [ i ] ) ;
        w->WaitFor ( ) ;
        delete w ;
    }
    FWorkerList->UnlockList ( ) ;
    FWorkerList->Clear ( ) ;
}
