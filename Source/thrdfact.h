//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: thrdfact.h $
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

    This class produces an asynchornous thread that creates as many worker
    threads as the user requires.  It tests to ensure that the new operator
    created the thread successfully before trying to apply notifyer addresses.

*/

#ifndef thrdfactH
#define thrdfactH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "waitthread.h"
#include "wkrthread.h"
#include "conndata.h"
//---------------------------------------------------------------------------
class ThreadFactory : public TWaitThread
{
    typedef Inherited TWaitThread ;

    private:
        TThreadList * FWorkerList   ;
        TStringList * FDSNConnList ; // attached connData objects

        int           FTestTime ;

        bool ODBCWkrs ( ) ;
        bool workerThreadCreate ( connData * ) ;
        void __fastcall KillAllThreads();

    protected:
        void __fastcall Execute ( ) ;

        bool ConnectivityTest ( ) ;
        void ConnectivityLoad ( ) ;                                      
        void WorkingLoad ( ) ;
        void Closure ( ) ;

    public:
        __fastcall ThreadFactory  ( TStringList * , int , TCriticalSection * , TNotifyLog ) ;
        __fastcall ~ThreadFactory ( ) ;

        void __fastcall Resume    ( ) ;
        void __fastcall Suspend   ( ) { Inherited::Suspend   ( ) ; }
        void __fastcall Terminate ( ) ; 

        // Event Handlers from worker threads
        void __fastcall WkrOnTerminate  ( TObject * ) ;
        __property int TestTime  = { read=FTestTime, write=FTestTime };

};
//---------------------------------------------------------------------------
#endif

