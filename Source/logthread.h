//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: logthread.h $
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

    This thread manages the visial and file logs for any component that
    requires this service.  The log shares a Critical Section with Log
    Event generators to ensure serialisation of each log line.

*/

#ifndef logthreadH
#define logthreadH
//---------------------------------------------------------------------------

#include <syncobjs.hpp>
#include <Classes.hpp>
//---------------------------------------------------------------------------
class TLogThread : public TThread
{
    private:
        TCriticalSection * FCS ;          // Shared with the Log Producers
        TStringList      * MessageQueue ;
        int                LogFile ;

        void    __fastcall syncLogEvent ( ) ;

    protected:
        void    __fastcall Execute ( ) ;

    public:
        __fastcall TLogThread ( ) ;
        virtual __fastcall ~TLogThread ( ) ;

        // Message Handler
        void    __fastcall LogEvent ( AnsiString ) ;
        __property TCriticalSection * CS = { read = FCS , write = FCS } ;
};
//---------------------------------------------------------------------------
#endif
