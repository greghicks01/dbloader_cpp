object TestTimeF: TTestTimeF
  Left = 164
  Top = 686
  Width = 262
  Height = 146
  Caption = 'Test Time'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object TimeSet: TEdit
    Left = 8
    Top = 32
    Width = 121
    Height = 21
    TabOrder = 0
    Text = '60'
  end
  object BitBtn1: TBitBtn
    Left = 32
    Top = 88
    Width = 75
    Height = 25
    TabOrder = 1
    Kind = bkCancel
  end
  object BitBtn2: TBitBtn
    Left = 136
    Top = 88
    Width = 75
    Height = 25
    TabOrder = 2
    Kind = bkOK
  end
  object TimeIntVal: TUpDown
    Left = 129
    Top = 32
    Width = 15
    Height = 21
    Associate = TimeSet
    Min = 0
    Max = 60
    Position = 60
    TabOrder = 3
    Wrap = False
  end
  object TimeInt: TRadioGroup
    Left = 152
    Top = 6
    Width = 89
    Height = 73
    Caption = 'Interval'
    ItemIndex = 0
    Items.Strings = (
      'Hours'
      'Minutes'
      'Seconds')
    TabOrder = 4
  end
end
