object frmLogin: TfrmLogin
  Left = 789
  Top = 567
  Width = 288
  Height = 229
  Caption = 'Connection Data'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 100
    Width = 53
    Height = 13
    Caption = 'User Name'
  end
  object Label2: TLabel
    Left = 8
    Top = 132
    Width = 46
    Height = 13
    Caption = 'Password'
  end
  object Label3: TLabel
    Left = 8
    Top = 40
    Width = 82
    Height = 13
    Caption = 'Max Connections'
  end
  object Label4: TLabel
    Left = 8
    Top = 71
    Width = 57
    Height = 13
    Caption = 'WaitInterval'
  end
  object Label5: TLabel
    Left = 8
    Top = 12
    Width = 56
    Height = 13
    Caption = 'ODBC DSN'
  end
  object UserNameEdit: TEdit
    Left = 64
    Top = 97
    Width = 193
    Height = 21
    TabOrder = 5
  end
  object PasswordEdit: TEdit
    Left = 64
    Top = 128
    Width = 193
    Height = 21
    TabOrder = 6
  end
  object CancelBitBtn: TBitBtn
    Left = 40
    Top = 164
    Width = 75
    Height = 25
    TabOrder = 7
    Kind = bkCancel
  end
  object OKButton: TBitBtn
    Left = 160
    Top = 164
    Width = 75
    Height = 25
    TabOrder = 8
    OnClick = OKButtonClick
    Kind = bkOK
  end
  object MaxConEdit: TEdit
    Left = 96
    Top = 36
    Width = 161
    Height = 21
    TabOrder = 1
    Text = '1'
  end
  object UpDown1: TUpDown
    Left = 257
    Top = 36
    Width = 10
    Height = 21
    Associate = MaxConEdit
    Min = 1
    Max = 1000
    Position = 1
    TabOrder = 2
    Wrap = False
  end
  object TimeoutEdit: TEdit
    Left = 96
    Top = 67
    Width = 161
    Height = 21
    TabOrder = 3
    Text = '10'
  end
  object UpDown2: TUpDown
    Left = 257
    Top = 67
    Width = 10
    Height = 21
    Associate = TimeoutEdit
    Min = 10
    Max = 14400
    Position = 10
    TabOrder = 4
    Wrap = False
  end
  object ODBCDSNEdit: TEdit
    Left = 72
    Top = 8
    Width = 185
    Height = 21
    Enabled = False
    TabOrder = 0
  end
end
