//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
*/
/*
 *  $Revision: 1.2 $
 *
 *  $Log: waitthread.h $
 *  Revision 1.2  2005/06/15 02:38:04  Administrator
 *  Added all internal documentation
 *
 *  Revision 1.1  2005/06/14 05:41:48  Administrator
 *  Initial revision
 *
 *
*/
/*

    This thread is the base class for Worker and Factory threads
    Many thnks to Malcom @ MJF for making this publicly available.

*/
/*!IM!
-------------------------------------------------------------------------
  Copyright Malcolm Smith, November 1998
  Email: malcolm@mjfreelancing.com
  W eb  : http://www.mjfreelancing.com

  The contents of this document and accompanying source code is
  Copyright © MJ Freelancing 2nd October 2002.  The source code is free
  to use on a royalty free basis in freeware, shareware and commercial
  applications as long as the following terms are met:

·	You make no claim to being the original author.

·	Where the code is used a reference to the original author, being
    Malcolm Smith, MJ Freelancing, is clearly stated in all source code
    and binary sources.

·	Where changes are made to the original source code these changes must
    be clearly marked as your alterations.

·	You use the code at your own risk and make your own assessment of the
    suitability of this code for any given purpose.

·	No warranty is given or implied.

·	No liability for implicit or explicit damages will be acknowledged or
    accepted by MJ Freelancing for any damages that arise through the use
    of this code.

History
~~~~~~~
02/10/2002 - v1.00 - Initial release (outside of MJFSecurity)
03/10/2002 - v1.01 - Small bug fixes (calling inherited methods) and improvements
-----------------------------------------------------------------------*/
//---------------------------------------------------------------------------
#ifndef waitthreadH
#define waitthreadH

#include <syncobjs.hpp>
#include <vcl.h>

class TWaitThread : public TThread
{
    typedef TThread Inherited ;

    // TNotifyLog - Added 2005
    typedef void __fastcall ( __closure * TNotifyLog ) ( AnsiString ) ;

    private:
        HANDLE hIntervalEvent ;
        DWORD FWaitInterval   ;
        TNotifyEvent FOnStart ;

        // Additional material 2005
        static AnsiString CopyRightString ;
        AnsiString  FThreadName ;
        TCriticalSection * FCS ; // shared from the log thread
        TNotifyEvent FOnTermSig ,
                     FOnFinishSig ;
        TNotifyLog   FLogEvent ;
        bool FCSEntered , FListLocked ;

    protected:
        virtual void __fastcall Execute(void);
        virtual void __fastcall BeforeProcess(void);
        virtual void __fastcall AfterProcess(void);
        // this is what is supposed to be overriden
        virtual void __fastcall Process(void);

        void __fastcall WaitForInterval();    // used by the Execute method in the derived class

        // additional functions
        void  __fastcall SetThreadName( AnsiString ) ;
        void __fastcall threadLogEvent( AnsiString ); // event log driver

    public:
        __fastcall TWaitThread();
        virtual __fastcall ~TWaitThread(void);

        // overriden terminate function
        void __fastcall Terminate(void);
        void __fastcall Finish(void);
        void __fastcall Resume(void);

        __property DWORD WaitInterval = { read = FWaitInterval, write = FWaitInterval } ;
        __property TNotifyEvent OnStart = { read = FOnStart, write = FOnStart } ;

        // additional 2005
        __property TNotifyEvent OnResume    = { read = FOnStart     , write = FOnStart     } ;
        __property TNotifyEvent OnTermSig   = { read = FOnTermSig   , write = FOnTermSig   } ;
        __property TNotifyEvent OnFinishSig = { read = FOnFinishSig , write = FOnFinishSig } ;
        __property TNotifyLog   LogEvent    = { read = FLogEvent    , write = FLogEvent    } ;
        
        __property TCriticalSection * CS = { read = FCS , write = FCS } ;
        __property bool        CSEntered = { read = FCSEntered , write = FCSEntered     } ;
        __property bool       ListLocked = { read = FListLocked , write = FListLocked   } ;
        __property AnsiString ThreadName = { read = FThreadName , write = SetThreadName } ;

};
//---------------------------------------------------------------------------
#endif
