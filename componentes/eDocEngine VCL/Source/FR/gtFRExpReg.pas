{***************************************************************************}
{                                                                           }
{  Gnostice eDocEngine 		                                                  }
{                                                                           }
{  Copyright � 2002-2008 Gnostice Information Technologies Private Limited  }
{  http://www.gnostice.com                                                  }
{                                                                           }
{***************************************************************************}

{------------------------------------}
{          Editor Options            }
{------------------------------------}
{                                    }
{ Tab Stops = 2                      }
{ Use Tab Character = True           }
{                                    }
{------------------------------------}

unit gtFRExpReg;

interface
uses
  Classes, gtFRXportIntf;

procedure Register;

implementation

procedure Register;
begin
	RegisterComponents('eDocEngine Additional', [TgtFRExportInterface]);
end;
end.
 