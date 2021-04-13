//---------------------------------------------------------------------------
/*

    This form get the user to inform the program of the desired test period.
    The selection requires an integer plus an interval identifier for
    hours, minutes or seconds.

*/

#ifndef TestTimeFrmH
#define TestTimeFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TTestTimeF : public TForm
{
__published:	// IDE-managed Components
    TEdit *TimeSet;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TUpDown *TimeIntVal;
    TRadioGroup *TimeInt;
private:	// User declarations
public:		// User declarations
    __fastcall TTestTimeF(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TTestTimeF *TestTimeF;
//---------------------------------------------------------------------------
#endif
