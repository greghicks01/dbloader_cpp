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
 *  $Log: waitthread.cpp $
 *  Revision 1.2  2005/06/15 02:38:43  Administrator
 *  Added all internal documentation
 *
 *  Revision 1.1  2005/06/14 05:45:26  Administrator
 *  Initial revision
 *
 *
*/
/*!IM!
-------------------------------------------------------------------------
  Copyright Malcolm Smith, November 1998
  Email: malcolm@mjfreelancing.com
  Web  : http://www.mjfreelancing.com

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

#include "waitthread.h"
#include <vcl.h>
#pragma hdrstop

#pragma package(smart_init)
AnsiString TWaitThread::CopyRightString = "Copyright for TWaitThread : © Malcolm Smith, MJ Freelancing";

__fastcall TWaitThread::TWaitThread() : TThread(true)
{
    // default to no-wait
    FWaitInterval = 0;

    // create the primary interval event
    // why not use TEvent? Because it's another memory allocation and
    // call for the EXACT same functionality
    hIntervalEvent = CreateEvent(NULL, true, false, NULL);
}
//---------------------------------------------------------------------------
__fastcall TWaitThread::~TWaitThread(void)
{
    if(hIntervalEvent)
    {
        CloseHandle(hIntervalEvent);
        hIntervalEvent = NULL;
    }
    threadLogEvent ( "---> Stopped work" ) ;
}
//---------------------------------------------------------------------------
void __fastcall TWaitThread::Resume(void)
{
    Inherited::Resume();

    if(FOnStart)
        FOnStart(this);
}
//---------------------------------------------------------------------------
void __fastcall TWaitThread::Finish(void)
{
    // not terminating -- telling the thread to finish
    threadLogEvent ( ">-- received Finish Signal --<" ) ;
    if(hIntervalEvent)
        SetEvent(hIntervalEvent);
}
//---------------------------------------------------------------------------
void __fastcall TWaitThread::Terminate(void)
{
    threadLogEvent ( ">-- received Terminate Signal --<" ) ;
    Inherited::Terminate();

    if(hIntervalEvent)
        SetEvent(hIntervalEvent);
}
//---------------------------------------------------------------------------
void __fastcall TWaitThread::WaitForInterval()
{
    // just reading one value -- shouldn't cause access problems
    // and we don't need to allocate and use a critical section
    DWORD tInterval = FWaitInterval;

    // wait for signal or timeout
    WaitForSingleObject(hIntervalEvent, tInterval);

    // reset the event
    ResetEvent(hIntervalEvent);
}
//---------------------------------------------------------------------------
// Execute() should not to be overriden unless you know what you're doing
//---------------------------------------------------------------------------
void __fastcall TWaitThread::Execute(void)
{
    //BeforeProcess();
    Process();
    //AfterProcess();
}
//---------------------------------------------------------------------------
void __fastcall TWaitThread::BeforeProcess(void)
{
  // override
}
//---------------------------------------------------------------------------
void __fastcall TWaitThread::Process(void)
{
    // default execute WaitForInterval - this is the method to override
    WaitForInterval ( ) ;
}
//---------------------------------------------------------------------------
void __fastcall TWaitThread::AfterProcess(void)
{
  // override
} 
//---------------------------------------------------------------------------

/*
 * Purpose :  Threads Single Point of Contact for logged events
 *             Derivatives of this thread can call once and
 *             have the entry stamped.
 *            Use of Critical Section ensures producers serialise
 *             their requests of the threaded log receiver.
 * Accepts :  Threads allocated string
 * Returns :  nothing
*/
void __fastcall TWaitThread::threadLogEvent( AnsiString s )
{
    //DONE: Add your source code here
    if ( FLogEvent )
    {
        FCS->Enter ( ) ;
        FCSEntered = true ;
        FLogEvent ( DateTimeToStr ( Now ( ) ) + " " + ThreadName + s ) ;
        FCS->Leave ( ) ;
        FCSEntered = false ;
    }
}
//---------------------------------------------------------------------------

/*
 * Purpose : Creates an artificial Thread Name based on the String handed
              in with a | character used to delineate components
 * Accepts : String with the items to use for the name
 * Returns : void
*/
void  __fastcall TWaitThread::SetThreadName( AnsiString d )
{
    AnsiString s = d.SubString( d.Pos ( "|" ) + 1 , d.Length()- d.Pos ( "|" ) + 1 ) ;
    FThreadName = d.SubString(1,5) + " : "
                  + s.StringOfChar ('0', 4 - s.Length ( ) )
                  + s + " : ";
}

