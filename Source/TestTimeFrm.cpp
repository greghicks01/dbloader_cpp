//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TestTimeFrm.h"
//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.1 $
 *
 *  $Log: TestTimeFrm.cpp $
 *  Revision 1.1  2005/06/15 02:38:43  Administrator
 *  Initial revision
 *
 *
*/
#pragma package(smart_init)
#pragma resource "*.dfm"
TTestTimeF *TestTimeF;
//---------------------------------------------------------------------------
/*
 * Purpose : CTOR
 * Accepts : Owner - pointer to upper container
 * Returns : TestTimeF
*/
__fastcall TTestTimeF::TTestTimeF(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
