//--------------------------------------------------------------------------- 
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: DSNList.cpp $
 *  Revision 1.2  2005/06/15 02:38:43  Administrator
 *  Added all internal documentation
 *
 *  Revision 1.1  2005/06/14 05:45:26  Administrator
 *  Initial revision
 *
 *
*/
//---------------------------------------------------------------------------
/*

    This form displays the enumerated ODBC sources and allows the user to
    select by drag and drop which sources are required for load testing.

*/

#include <vcl.h>
#pragma hdrstop

#include "DSNList.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDSNListFrm *DSNListFrm;
//---------------------------------------------------------------------------
__fastcall TDSNListFrm::TDSNListFrm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

/*
 * Purpose : Drag and Drop initiator
 * Accepts : Sender - pointer to object under mouse
 *           Button - WM_MouseButton
 *           Shift  - WM_KeyboardShiftState
 *           X Y    - Mouse position (pixels)
 * Returns : Nil
*/
void __fastcall TDSNListFrm::DSNListBoxMouseDown(TObject *Sender ,
      TMouseButton Button , TShiftState Shift , int X , int Y )
{
      if ( Button != mbLeft ) return ;

      TListBox * listBox = static_cast< TListBox * > ( Sender ) ;

      if ( listBox->ItemAtPos ( Point ( X , Y ) , true ) >= 0 )
           listBox->BeginDrag ( false , 5 ) ;
}
//---------------------------------------------------------------------------
    
/*
 * Purpose : Drag and Drop - Drag Over Accept on right target
 * Accepts : Sender - pointer to object under mouse
 *           Source - pointer to object where mouse down started
 *           X Y    - Mouse position (pixels)
 *           State  - DragDrop State - T/F
 *           Accept - Determines if the drop can be completed over Sender
 * Returns : Nil
*/
void __fastcall TDSNListFrm::ListBox2DragOver(TObject *Sender ,
      TObject *Source , int X , int Y , TDragState State , bool &Accept)
{
      if ( Source == DSNListBox )
          Accept = true ;
      else
          Accept = false ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Drop while in accepted area
 * Accepts : Sender - pointer to object under mouse
 *           Source - pointer to object where mouse down started
 *           X Y    - Mouse position (pixels)
 * Returns : Nil
*/
void __fastcall TDSNListFrm::DSNListBoxDragDrop(TObject *Sender,
      TObject *Source, int X, int Y)
{
      if ( Sender == Source ) return ;

      TListBox * sender = static_cast < TListBox * > ( Sender ) ;
      TListBox * source = static_cast < TListBox * > ( Source ) ;

      sender->Items->Add     ( source->Items->Strings [ source->ItemIndex ]  ) ;
      source->Items ->Delete ( source->ItemIndex ) ;

      if ( sender->Items->Count > 0 )
      {
         BitBtn2->Enabled = true ;
      }
}
//---------------------------------------------------------------------------

