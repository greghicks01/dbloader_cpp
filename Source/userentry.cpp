//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: userentry.cpp $
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

#include "userentry.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmLogin *frmLogin;
//---------------------------------------------------------------------------
__fastcall TfrmLogin::TfrmLogin(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

/*
 * Purpose : Traps blank Password or Username
             - prevents exiting from form in Modal
 * Accepts : Sender - pointer to message initiator
 * Returns : Nil
*/
//---------------------------------------------------------------------------
void __fastcall TfrmLogin::OKButtonClick(TObject *Sender)
{
    // intercept blank IUD and Password
    if ( UserNameEdit->Text.Length ( ) == 0 || PasswordEdit->Text.Length ( ) == 0)
    {
        Application->MessageBoxA ( "User Name and Password cannot be blank" , "Error",MB_OK ) ;
        ModalResult = mrNone ;
    }
}
//---------------------------------------------------------------------------

/*
 * Purpose : Method to enable the connData information to be filled on the form
 * Accepts : * connData wrapper
 * Returns : Nil
*/
void __fastcall TfrmLogin::loadConnData( connData * c )
{
    //DONE: Add your source code here
    c->SetDSN( ODBCDSNEdit->Text ,
               UserNameEdit->Text ,
               PasswordEdit->Text ) ;

    c->MaxCons  = StrToInt ( MaxConEdit->Text ) ;
    c->Timeout  = StrToInt ( TimeoutEdit->Text ) ;
}
