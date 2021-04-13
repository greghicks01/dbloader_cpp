object LaunchTimeSet: TLaunchTimeSet
  Left = 417
  Top = 718
  BorderStyle = bsDialog
  Caption = 'Launch Interval'
  ClientHeight = 75
  ClientWidth = 168
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
  object LaunchIntervalEdit: TEdit
    Left = 16
    Top = 8
    Width = 121
    Height = 21
    TabOrder = 0
    Text = '500'
  end
  object UpDown1: TUpDown
    Left = 137
    Top = 8
    Width = 10
    Height = 21
    Associate = LaunchIntervalEdit
    Min = 10
    Max = 1000
    Position = 500
    TabOrder = 1
    Thousands = False
    Wrap = False
  end
  object BitBtn1: TBitBtn
    Left = 40
    Top = 40
    Width = 75
    Height = 25
    TabOrder = 2
    Kind = bkOK
  end
end
