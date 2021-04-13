//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: dbloader.cpp $
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
USERES("dbloader.res");
USEUNIT("conndata.cpp");
USEUNIT("thrdfact.cpp");
USEUNIT("wkrthread.cpp");
USEUNIT("waitthread.cpp");
USEFORM("mainfrm.cpp", frmMainFrm);
USEFORM("userentry.cpp", frmLogin);
USEFORM("LaunchTimer.cpp", LaunchTimeSet);
USEFORM("DSNList.cpp", DSNListFrm);
USEUNIT("logthread.cpp");
USEFORM("TestTimeFrm.cpp", TestTimeF);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
            Application->Initialize();
            Application->Title = "ODBC Connection and Server Load Test";
            Application->CreateForm(__classid(TfrmMainFrm), &frmMainFrm);
                 Application->Run();
        }
        catch (Exception &exception)
        {
            Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
