//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: mainfrm.cpp $
 *  Revision 1.2  2005/06/15 02:38:43  Administrator
 *  Added all internal documentation
 *
 *  Revision 1.1  2005/06/14 05:45:26  Administrator
 *  Initial revision
 *
 *
*/
//---------------------------------------------------------------------------

#include <vcl.h> // for interface items
#pragma hdrstop

#include "mainfrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmMainFrm *frmMainFrm;

// constants to access the machines registry
const AnsiString s   = "Software\\ODBC\\ODBC.INI\\ODBC Data Sources" ;
const HKEY       k[] = { HKEY_CURRENT_USER , HKEY_LOCAL_MACHINE } ;

//---------------------------------------------------------------------------

/*
 * Purpose : CTOR
 * Accepts : Pointer to owner
 * Returns : TfrmMainFrm *
*/
__fastcall TfrmMainFrm::TfrmMainFrm(TComponent* Owner)
        : TForm ( Owner )
{
    ODBCSourceList  = new TStringList ;
    FDSNList        = new TStringList ;
    LogThread       = new TLogThread  ;
    LogThread->Resume ( ) ;
    ThreadFac = NULL ;
    FLaunchInterval = 500 ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : DTOR
 * Accepts : Nil
 * Returns : Nil
*/
__fastcall TfrmMainFrm::~TfrmMainFrm ( )
{
    //DONE: Add your source code here - this must not run until wkr and Factory are completely finished.
    // clean up connData objects
    for ( int i = 0 ; i < FDSNList->Count ; delete FDSNList->Objects [i] , i++ ) ;
    delete FDSNList  ;
    delete ThreadFac ;

    for ( int i = 0 ; i < ODBCSourceList->Count ; delete ODBCSourceList->Objects [i] , i++ ) ;
    delete ODBCSourceList ;

    // LogThread must be active till the absolute end
    LogThread->Terminate ( ) ;
    LogThread->Resume    ( ) ;
    LogThread->WaitFor   ( ) ;
    delete LogThread ;

    OperationalLog->Lines->Clear ( ) ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Prepares ODBC source on this event
 * Accepts : Sender  - pointer to item requesting this event
 * Returns : Nil
*/
void __fastcall TfrmMainFrm::FormShow(TObject *Sender)
{
    //DONE: Enum ODBC Sources
    enumODBCSources ( ) ;
}
//---------------------------------------------------------------------------
 
/*
 * Purpose : opens DSNList for user to select from DSN list, then collects
 *            data for DSN UID, PWD plus maxthreads and launchinterval
 * Accepts : pointer to object sending the message
 * Returns : Nil
*/
void __fastcall TfrmMainFrm::SelectConnection1Click ( TObject * Sender )
{
    //DONE: Select and prepare connection strings on selected items
    TDSNListFrm * DSNListFrm ;
    Application->CreateForm ( __classid ( TDSNListFrm ) , &DSNListFrm ) ;

    DSNListFrm->DSNListBox->Items->AddStrings ( ODBCSourceList ) ;

    if ( DSNListFrm->ShowModal ( ) == mrOk )
    {
        for ( int i = 0 ; i < DSNListFrm->ListBox2->Items->Count ; ++i )
        {
            TfrmLogin  * frmLogin ;
            Application->CreateForm ( __classid ( TfrmLogin ) , & frmLogin )         ;

            frmLogin->ODBCDSNEdit->Text = DSNListFrm->ListBox2->Items->Strings [ i ] ;

            if ( frmLogin->ShowModal ( ) == mrOk )
            {
                connData * ConnData = new connData ;
                frmLogin->loadConnData ( ConnData ) ;
                
                FDSNList->AddObject ( ConnData->DSN , reinterpret_cast < TObject * > ( ConnData ) );
            } // if

            frmLogin->Close ( ) ;
        } // for
        
        DBConnBox->Items->AddStrings ( FDSNList ) ;
    } // if

    DSNListFrm->Close ( ) ;
    delete DSNListFrm ;
}
//---------------------------------------------------------------------------

/*
 * Purpose :  Factory Lauch Setting
 * Accepts :  Object initiating message
 * Returns :  Nil
*/
void __fastcall TfrmMainFrm::TimeInterval1Click(TObject *Sender)
{
    // set timer interval
    TLaunchTimeSet * LaunchTimeSet ;
    Application->CreateForm ( __classid ( TLaunchTimeSet ) , &LaunchTimeSet ) ;

    LaunchTimeSet->ShowModal ( ) ;

    FLaunchInterval = StrToInt ( LaunchTimeSet->UpDown1->Position ) ;

    LaunchTimeSet->Close ( ) ;

    delete LaunchTimeSet ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Enumerates ODBC sources from two registry Keys into a single list
              HKEY_LOCAL_MACHINE
              HKEY_CURRENT_USER
 * Accepts :  Nothing
 * Returns :  Nothing
*/
void __fastcall TfrmMainFrm::enumODBCSources ( )
{
    TStringList * l = new TStringList ;
    TRegistry * r = new TRegistry ( KEY_READ ) ;

    for ( int i = 0 ; i < 2 ; i++ ) {

        r->RootKey = k [ i ] ;
        TraverseKeys ( r , s ) ;
        r->GetValueNames ( l ) ;

        ODBCSourceList->AddStrings ( l ) ;
    } // for

    delete l;
    delete r ;

    ODBCSourceList->Sort ( ) ;
}
//---------------------------------------------------------------------------
 
/*
 * Purpose : Field setter
 * Accepts : StringList *
 * Returns : Nil
*/
void __fastcall TfrmMainFrm::SetODBCSourceList ( TStringList * value )
{
    if ( FODBCSourceList != value ) {
        FODBCSourceList = value;
    }
}
//---------------------------------------------------------------------------

/*
 * Purpose : Field getter
 * Accepts : Nil
 * Returns : * String List
*/
TStringList * __fastcall TfrmMainFrm::GetODBCSourceList ( )
{
    return FODBCSourceList;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Traverses Registry keys
              Algorithm breaks \ delimited key up to enable traverse
              until the last string when s is set to NULL.
              The last element of s requires the strategy does not have
              the delimiter and requires the else statements to manage.
 * Accepts :  pointer to registry and string key path
 * Returns :  registry set to current
*/
bool TfrmMainFrm::TraverseKeys(TRegistry * r , const AnsiString s )
{
    //DONE: Add your source code here
    AnsiString keys = s ;
    AnsiString delimiter = "\\" ;
    bool t = true;

    while ( keys.Length ( ) != 0 && t ) {
        AnsiString k ;

        if ( keys.Pos(delimiter) != 0)
        {
            k = keys.SubString ( 1 , keys.Pos( delimiter ) - 1 ) ;
            keys = keys.SubString ( k.Length ( ) + 2 , 
                       keys.Length ( ) - ( k.Length ( )+ 1 ) ) ;
        }
        else {
            k = keys ;
            keys = "" ;
        } // if
        t = r->OpenKey ( k , false ) ;
    } // while
    
    return t ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Sets DSN List
 * Accepts : * String List
 * Returns : Nil
*/
void __fastcall TfrmMainFrm::SetDSNList(TStringList * value)
{
    if(FDSNList != value) {
        FDSNList = value ;
    }
}
//---------------------------------------------------------------------------

/*
 * Purpose : Returns DSN List
 * Accepts : Nil
 * Returns : * String List
*/
TStringList * __fastcall TfrmMainFrm::GetDSNList()
{
    return FDSNList;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Start the tests
 * Accepts : * object initiating message
 * Returns : Nil
*/
void __fastcall TfrmMainFrm::Start1Click(TObject *Sender)
{
    TTestTimeF * f ;
    Application->CreateForm ( __classid ( TTestTimeF) , &f ) ;
    if ( f->ShowModal ( ) == mrOk )
    {
        ThreadFac = new ThreadFactory ( FDSNList , FLaunchInterval , LogThread->CS , &LogThread->LogEvent ) ;
        int interval = 0;
        switch ( f->TimeInt->ItemIndex )
        {
            case 0 :
                interval = 3600 ;
                break ;
            case 1 :
                interval = 60 ;
                break ;
            case 2 :
                interval = 1 ;
                break ;
        }
        ThreadFac->TestTime = StrToInt ( f->TimeIntVal->Position ) * interval * 1000 ;
        ThreadFac->Resume ( ) ;
    }
    f->Close ( );
    delete f ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Termination notification receptor
 * Accepts : Object Initiating the message
 * Returns : Nil
*/
void __fastcall TfrmMainFrm::OnTerminated ( TObject * Sender )
{
     // Thread Factory terminated - can only happen when worker are all freed

}
//---------------------------------------------------------------------------

/*
 * Purpose : Prevents accidental closure and prevent DTOR until all workers
 *            and factory are done.
 * Accepts : Sender pointer to Object sending message
 *           CanClose reference to application variable
 * Returns : Nil
*/
void __fastcall TfrmMainFrm::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
    //DONE: Ask user to confirm then wait for thread fac before closing
    if ( ThreadFac != NULL )
    {
        if (  Application->MessageBoxA ( "Are you sure" , "Cancel Operation in Progress" , MB_OKCANCEL )== IDOK )
        {
            ThreadFac->Terminate ( ) ;
            ThreadFac->Resume    ( ) ;
            ThreadFac->Finish    ( ) ;
            ThreadFac->WaitFor   ( ) ;
        }
        else
            CanClose = false ;
    } // ThreadFac==NULL
}
//---------------------------------------------------------------------------

/*
 * Purpose : User clicks on File|Exit
 * Accepts : Sender pointer to Object sending message
 * Returns : Nil
*/
void __fastcall TfrmMainFrm::frmExitMenuClick(TObject *Sender)
{
    Close ( ) ;    
}
//---------------------------------------------------------------------------

