//---------------------------------------------------------------------------
/*
 *                     Copyright © 2005
 *                 Software Project Engineer
 *                 Mr Gregory Hicks BIT CQU
 *
 *  $Revision: 1.2 $
 *
 *  $Log: conndata.cpp $
 *  Revision 1.2  2005/06/15 02:38:43  Administrator
 *  Added all internal documentation
 *
 *  Revision 1.1  2005/06/14 05:45:26  Administrator
 *  Initial revision
 *
 *  
*///---------------------------------------------------------------------------

#include <vcl.h>
#include "conndata.h"
#pragma hdrstop
                
#pragma package(smart_init)
//---------------------------------------------------------------------------

/*
 * Purpose : Copy CTOR
 * Accepts : connData wrapper
 * Returns : connData
*/
TconnData::TconnData   ( const TconnData & c )
{
   FMaxCons   = c.FMaxCons  ;
   FTimeout   = c.FTimeout  ;
   FDSNConnString = c.FDSNConnString;

   FRunCount  = c.FRunCount ;
   FNewFail   = c.FNewFail  ;
   FConFail   = c.FConFail  ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : operator=
 * Accepts : connData wrapper
 * Returns : connData containing c contents
*/
TconnData & TconnData::operator= ( const TconnData & c )
{
   FMaxCons   = c.FMaxCons  ;
   FDSNConnString= c.FDSNConnString ;
   FTimeout   = c.FTimeout  ;

   FRunCount  = c.FRunCount ;
   FNewFail   = c.FNewFail  ;
   FConFail   = c.FConFail  ;
   
   return * this ;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Sets maximum connections
 * Accepts : value
 * Returns : Nil
*/
void __fastcall TconnData::SetMaxCons(int value)
{
    if(FMaxCons != value) {
        FMaxCons = value;
    }
}
//---------------------------------------------------------------------------

/*
 * Purpose : Retrieves the maximum value from private member
 * Accepts : Nil
 * Returns : integer
*/
int __fastcall TconnData::GetMaxCons()
{
    return FMaxCons;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Sets a private member value
 * Accepts : Integer
 * Returns : Nil
*/
void __fastcall TconnData::SetTimeout(int value)
{
    if(FTimeout != value) {
        FTimeout = value;
    }
}
//---------------------------------------------------------------------------

/*
 * Purpose : Gets provate member value
 * Accepts : Nil
 * Returns : Integer
*/
int __fastcall TconnData::GetTimeout()
{
    return FTimeout;
}
//---------------------------------------------------------------------------

/*
 * Purpose : Sets the private member to form a connection string
 * Accepts : DSN  Database Source Name
 *           UID  User ID
 *           PW   Password
 * Returns :
*/
void __fastcall TconnData::SetDSN ( AnsiString DSN ,AnsiString UID , AnsiString PW )
{
    FDSNConnString = "DSN="  + DSN
                   + ";UID=" + UID
                   + ";PWD=" + PW ;
}
