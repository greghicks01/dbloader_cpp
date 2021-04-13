//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: DSNList.h $
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

    This form shows the user the available DSN and allows them to select
    the one we want to test.

*/

#ifndef DSNListH
#define DSNListH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TDSNListFrm : public TForm
{
    __published:	// IDE-managed Components
        TListBox *DSNListBox;
        TListBox *ListBox2;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TStatusBar *StatusBar1;
        TLabel *Label1;
        TLabel *Label2;
        void __fastcall DSNListBoxMouseDown(TObject *Sender, TMouseButton Button,
                TShiftState Shift, int X, int Y);
        void __fastcall ListBox2DragOver(TObject *Sender, TObject *Source,
                int X, int Y, TDragState State, bool &Accept);
        void __fastcall DSNListBoxDragDrop(TObject *Sender, TObject *Source,
                int X, int Y);
    private:	// User declarations

    public:		// User declarations
        __fastcall TDSNListFrm(TComponent* Owner);

};
//---------------------------------------------------------------------------
extern PACKAGE TDSNListFrm *DSNListFrm;
//---------------------------------------------------------------------------
#endif
