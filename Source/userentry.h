//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: userentry.h $
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

    This form gathers the basic connection information, like UID, PWD and
    maximum connections and the rest time between each iteration of the
    executed code.

*/

#ifndef userentryH
#define userentryH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp> 
#include "conndata.h"
//---------------------------------------------------------------------------
class TfrmLogin : public TForm
{
    __published:	// IDE-managed Components
        TLabel  * Label1;
        TLabel  * Label2;
        TLabel  * Label3;
        TLabel  * Label4;
        TLabel  * Label5;
        TEdit   * UserNameEdit;
        TEdit   * PasswordEdit;
        TBitBtn * OKButton;
        TBitBtn * CancelBitBtn;
        TUpDown * UpDown1;
        TEdit   * MaxConEdit; 
        TUpDown * UpDown2;
        TEdit   * TimeoutEdit;
        TEdit   * ODBCDSNEdit;
        void __fastcall OKButtonClick ( TObject * Sender );

    private:	// User declarations

    public:		// User declarations
        __fastcall TfrmLogin  ( TComponent * ) ;
        void __fastcall loadConnData ( connData * ) ;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLogin *frmLogin;
//---------------------------------------------------------------------------
#endif
