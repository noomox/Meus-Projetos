/*
This example shows how to generate a machine modifier at run time
while using the TOgSerialNumber component. This means the program
will run only on the machine being used to run the application.

If the program is being run the first time (as determined by the
existance of an INI file, a dialog box is displayed showing the
modifier value along with two entry fields: one for the serial
number, the other for the release code. The user enters the
values that you give him for these two items. The values are
stored in the INI file and the program is allowed to run.

The release code you give the user can be generated by the CODEGEN
example program. Be sure to select the same type component, the
key used to compile the application (the one returned in the
OnGetKey event), and enter the machine modifier the user gives
you - not the one automatically generated when you click the
MachineModifier radio button. Also, if used, make sure the
Expires date is the same.
*/
//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#include "ExDModU1.h"
#include "ExDModU2.h"
//---------------------------------------------------------------------------
#pragma link "OnGuard"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CloseBtnClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OgSerialNumberCode1GetKey(TObject *Sender, TKey &Key)
{
  memcpy(&Key, CKey, sizeof(TKey));
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OgSerialNumberCode1GetModifier(TObject *Sender,
	int &Value)
{
  // Generate the value unique to this machine
  Value = GenerateMachineModifierPrim();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OgSerialNumberCode1GetCode(TObject *Sender, TCode &Code)
{
  // force the INI file to be in the same directory as the application}
  TheDir = ExtractFilePath(ParamStr(0));
  int L = TheDir.Length();
  if (L > 3 && TheDir[L] != '\\')
    TheDir = TheDir + '\\';

  /*
  this check helps prevent an empty INI file. For the combination of
  this component and using the MachineModifier, an empty or
  incomplete INI file can cause problems, i.e., if something goes wrong
  during the initial input of the SN and ReleaseCode, the program could
  not be run again without deleting the INI file. In a "real world"
  application, you would probably be hiding these values some how and
  would either have to tell the user where to delete things or have a
  utility program to do so.
  */
  if (!FileExists(TheDir + "SNWMOD.INI"))
    return;

  // open Ini File}
  IniFile = new TIniFile(TheDir + "SNWMOD.INI");
  try {
    // try to read release code}
    String S1 = IniFile->ReadString("Codes", "SNCode", "");

    /*
    set a global variable to the value of the serial number.
    Since, by design, the TOgSerialNumber component does not
    confirm that the SN and ReleaseCode retrieved from the INI
    file are compatible, this value will be used later to
    see if the user has attempted to change the contents of
    the INI file
    */
    IniSNVal = IniFile->ReadInteger("Codes", "SN", 0);

    // convert retrieved string to a code}
    HexToBuffer(S1, &Code, sizeof(TCode));
  }
  catch (...) {
  	delete IniFile;
    IniFile = 0;
    return;
  }
  delete IniFile;
  IniFile = 0;
}
//---------------------------------------------------------------------------
int TForm1::GetSNData(String& S)
{
  TCode TC;
  String SNC;
  int L;
  TKey Key;
  int Result;

	TSNEntryDlg* dlg = new TSNEntryDlg(Application);
  try {
    // Display new SN and ask for release code
    dlg->SNText->Text = "";
    dlg->CodeText->Text = "";

    Result = dlg->ShowModal();
    if (dlg->CodeText->Text == "" ||
       dlg->SNText->Text == "")
      Result = mrCancel;
    if (Result == mrCancel) {
      S = "Invalid Code or Cancelled";
      return Result;
    }

    // Check that Release Code was entered correctly
    memcpy(&Key, CKey, sizeof(TKey));
    OgSerialNumberCode1->Modifier = dlg->ModString->Text;

    HexToBuffer(dlg->ModString->Text, &L, sizeof(L));

    HexToBuffer(dlg->CodeText->Text, &TC, sizeof(TCode));
    ApplyModifierToKeyPrim(L, &Key, sizeof(TKey));

    if (!IsSerialNumberCodeValid(Key, TC)) {
      S = "Release code not entered correctly";
      Result = mrCancel;
    }
    else {
      IniFile = new TIniFile(TheDir + "SNWMOD.INI");
      try {
        // write SN to IniFile
        IniSNVal = dlg->SNText->Text.ToInt();
        SNC = dlg->CodeText->Text;
        IniFile->WriteInteger("Codes", "SN", IniSNVal);
        IniFile->WriteString("Codes", "SNCode", SNC);
      }
      catch (...) {
      	delete IniFile;
        IniFile = 0;
      }
		}
  }
  catch (...) {
    delete dlg;
    dlg = 0;
  }
  delete dlg;
  return Result;
}

void __fastcall TForm1::OgSerialNumberCode1Checked(TObject *Sender,
	TCodeStatus Status)
{
	int LI;
  String S;
  switch (Status) {
    case ogValidCode   : {
      // check if retrieved Serial Number matches Code
      LI = OgSerialNumberCode1->GetValue();

      if (LI != IniSNVal) {
        S = "The serial number has been changed";
			}
      else {
        Label1->Caption = AnsiString("Serial #: ") + IniSNVal;
        return;
      }
      break;
    }
    case ogInvalidCode : {
      // if INI file doesn't exist, presume this is first run
      if (!FileExists(TheDir + "SNWMOD.INI")) {
        if (GetSNData(S) != mrCancel) {
          // Check the SN/ReleaseCode
          OgSerialNumberCode1->CheckCode(true);
          // must Exit since line above began a recursive call
          return;
        }
			}
      else
        S = "Invalid Code";
      break;
    }
    case ogCodeExpired : {
    	S = "Evaluation period expired";
      break;
    }
  }

  ShowMessage(S);
  Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
	if (IniFile) delete IniFile;
}
//---------------------------------------------------------------------------