//--------------------------------------------------------------------------- 
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: LaunchTimer.cpp $
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

#include "LaunchTimer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLaunchTimeSet *LaunchTimeSet;
//---------------------------------------------------------------------------

/*
 * Purpose : CTOR
 * Accepts : Owner - pointer to upper container
 * Returns : TLaunchTimeSet
*/
__fastcall TLaunchTimeSet::TLaunchTimeSet(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

