//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: LaunchTimer.h $
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

    Form collects the time in milliseconds from the user to control launches
    of the Worker Threads.

*/

#ifndef LaunchTimerH
#define LaunchTimerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TLaunchTimeSet : public TForm
{
    __published:	// IDE-managed Components
        TEdit *LaunchIntervalEdit;
        TUpDown *UpDown1;
        TBitBtn *BitBtn1;
        
    private:	// User declarations

    public:		// User declarations
        __fastcall TLaunchTimeSet(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLaunchTimeSet *LaunchTimeSet;
//---------------------------------------------------------------------------
#endif
