//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                   
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: conndata.h $
 *  Revision 1.2  2005/06/15 02:38:04  Administrator
 *  Added all internal documentation
 *
 *  Revision 1.1  2005/06/14 05:41:48  Administrator
 *  Initial revision
 *
 *
*/
//---------------------------------------------------------------------------

#ifndef conndataH
#define conndataH

#include <sqlext.h>
#include <sql.h>
#include <vcl.h>

/*

    This class contains the information the user entered and allows
    the program to use the flag set to enable detection of issues
    like New Fail and Connection Fail.
    
*/

class TconnData : public TObject
{
    private:
        // User Entries
        int        FMaxCons  ,      // Maximum Connections
                   FTimeout  ;      // DB Timeout value (secs)
        AnsiString FDSNConnString ; // the ODBC String
        
        // Control Flags and counters
        int        FRunCount ; // Qty running threads
        bool       FNewFail  ; // New failed to allocated
        bool       FConFail  ; // Connection fail flag

        void __fastcall SetMaxCons ( int value ) ;
        int  __fastcall GetMaxCons ( ) ;

        void __fastcall SetTimeout ( int value ) ;
        int  __fastcall GetTimeout ( ) ;

        AnsiString __fastcall GetDSN ( ) ;

    protected:

    public:
        TconnData   ( ) : FMaxCons(0),FTimeout(0),
                          FRunCount(0),FNewFail(false),FConFail(false) { } ;
        virtual __fastcall ~TconnData ( ) { } ;
         
        void       __fastcall SetDSN ( AnsiString DSN ,AnsiString UID , AnsiString PW ) ;
        
        TconnData                     ( const TconnData & ) ;
        TconnData & operator=         ( const TconnData & ) ;

        __property int        MaxCons   = { read = GetMaxCons  , write = SetMaxCons  } ;
        __property int        Timeout   = { read = GetTimeout  , write = SetTimeout  } ;
        // set FDSNConnString
        __property AnsiString DSN       = { read = FDSNConnString  , nodefault } ;
        __property int        RunCount  = { read = FRunCount     , write = FRunCount } ;
        __property bool       NewFail   = { read = FNewFail      , write = FNewFail  } ;
        __property bool       ConFail   = { read = FConFail      , write = FConFail  } ;
} ;
//---------------------------------------------------------------------------

#endif
