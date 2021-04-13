//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: mainfrm.h $
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

    Main form - controls all application operation.  The log thread is the
    first running thread, the Factory is not created until the Start menu item
    is clicked.

*/

#ifndef mainfrmH
#define mainfrmH
//---------------------------------------------------------------------------
#include "logthread.h"    // logging thread
#include "DSNList.h"      // Shows the DSN List for the user to select from
#include "LaunchTimer.h"  // Time between Launches of Worker Threads
#include "userentry.h"    //
#include "conndata.h"     // data wrapper
#include "thrdfact.h"
#include "TestTimeFrm.h"  // Test Timer setting

#include <Registry.hpp>  // TRegistry
#include <Menus.hpp>     // Main Form Menu
#include <Forms.hpp>
#include <Classes.hpp>   // Standard Classes
#include <Controls.hpp>  // Controls
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmMainFrm : public TForm
{
    __published:	// IDE-managed Components
        TPanel      * Panel1 ;
        TPanel      * Panel3 ;
        TMainMenu   * MainMenu1 ;
        TMenuItem   * frmFileMenu ;
        TMenuItem   * frmODBCMenu ;
        TMenuItem   * frmExitMenu ;
        TMenuItem   * Start1 ;
        TMenuItem   * Loading ;
        TGroupBox   * DBConnGrpBox ;
        TListBox    * DBConnBox ;
        TGroupBox   * WkrThreadListGrpBox ;
        TListBox    * WorkerThreadList ;
        TGroupBox   * OpertaionLogGrpBox ;
        TMemo       * OperationalLog ;
        TStatusBar  * StatusBar1 ;

        void __fastcall FormShow               ( TObject * Sender ) ;
        void __fastcall SelectConnection1Click ( TObject * Sender ) ;
        void __fastcall TimeInterval1Click     ( TObject * Sender ) ;
        void __fastcall Start1Click            ( TObject * Sender ) ;
        void __fastcall FormCloseQuery         ( TObject * Sender , bool &CanClose ) ;
    void __fastcall frmExitMenuClick(TObject *Sender);

    protected:
        int FLaunchInterval;

    private:
        ThreadFactory  * ThreadFac ;
        TLogThread     * LogThread ;
        TStringList    * FODBCSourceList ;
        TStringList    * FDSNList ;

        void __fastcall SetODBCSourceList ( TStringList * value ) ;
        TStringList    * __fastcall GetODBCSourceList ( ) ;	// User declarations

        void __fastcall enumODBCSources ( ) ;
        virtual __fastcall ~TfrmMainFrm ( ) ;
        bool TraverseKeys ( TRegistry *, const AnsiString ) ;

        TStringList * __fastcall GetDSNList ( ) ;
        void __fastcall SetDSNList ( TStringList * value ) ;

        __property TStringList * ODBCSourceList  = { read=GetODBCSourceList, write=SetODBCSourceList } ;
        __property TStringList * DSNList  = { read=GetDSNList, write=SetDSNList } ;

    public:		// User declarations
        __fastcall TfrmMainFrm ( TComponent* Owner ) ;

        //Logevent Manager
        void __fastcall OnTerminated ( TObject * ) ;
        void __fastcall TestStep ( TObject * ) ;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMainFrm *frmMainFrm;
//---------------------------------------------------------------------------

#endif
