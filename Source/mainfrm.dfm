object frmMainFrm: TfrmMainFrm
  Left = 247
  Top = 180
  Width = 797
  Height = 595
  Caption = 'DB Load Tester'
  Color = clBtnFace
  Constraints.MaxHeight = 595
  Constraints.MaxWidth = 797
  Constraints.MinHeight = 595
  Constraints.MinWidth = 797
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCloseQuery = FormCloseQuery
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 233
    Height = 530
    Align = alLeft
    TabOrder = 0
    object DBConnGrpBox: TGroupBox
      Left = 8
      Top = 8
      Width = 217
      Height = 89
      Caption = 'DB Connections'
      TabOrder = 0
      object DBConnBox: TListBox
        Left = 8
        Top = 16
        Width = 201
        Height = 65
        ItemHeight = 13
        TabOrder = 0
      end
    end
    object WkrThreadListGrpBox: TGroupBox
      Left = 8
      Top = 96
      Width = 217
      Height = 425
      Caption = 'Worker Threads'
      TabOrder = 1
      object WorkerThreadList: TListBox
        Left = 8
        Top = 16
        Width = 201
        Height = 401
        ItemHeight = 13
        TabOrder = 0
      end
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 530
    Width = 789
    Height = 19
    Panels = <>
    SimplePanel = False
  end
  object Panel3: TPanel
    Left = 233
    Top = 0
    Width = 556
    Height = 530
    Align = alClient
    Caption = 'Panel3'
    TabOrder = 2
    object OpertaionLogGrpBox: TGroupBox
      Left = 8
      Top = 8
      Width = 545
      Height = 513
      Caption = 'Operation Log'
      TabOrder = 0
      object OperationalLog: TMemo
        Left = 8
        Top = 16
        Width = 529
        Height = 489
        Enabled = False
        TabOrder = 0
      end
    end
  end
  object MainMenu1: TMainMenu
    Left = 24
    Top = 32
    object frmFileMenu: TMenuItem
      Caption = 'File'
      object frmExitMenu: TMenuItem
        Caption = 'Exit'
        OnClick = frmExitMenuClick
      end
    end
    object frmODBCMenu: TMenuItem
      Caption = 'ODBC'
      OnClick = SelectConnection1Click
    end
    object Loading: TMenuItem
      Caption = 'Time Interval'
      OnClick = TimeInterval1Click
    end
    object Start1: TMenuItem
      Caption = 'Start'
      OnClick = Start1Click
    end
  end
end
