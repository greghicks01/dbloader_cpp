object DSNListFrm: TDSNListFrm
  Left = 439
  Top = 394
  Width = 396
  Height = 412
  Caption = 'DSN List'
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
    Left = 24
    Top = 8
    Width = 115
    Height = 13
    Caption = 'Local Machine DSN List'
  end
  object Label2: TLabel
    Left = 240
    Top = 8
    Width = 58
    Height = 13
    Caption = 'User Choice'
  end
  object DSNListBox: TListBox
    Left = 8
    Top = 32
    Width = 169
    Height = 297
    Hint = 'Drag DSN'#39's to test to User Choice'
    ItemHeight = 13
    MultiSelect = True
    Sorted = True
    TabOrder = 0
    OnDragDrop = DSNListBoxDragDrop
    OnDragOver = ListBox2DragOver
    OnMouseDown = DSNListBoxMouseDown
  end
  object ListBox2: TListBox
    Left = 208
    Top = 32
    Width = 169
    Height = 297
    Hint = 'Drag DSN'#39's you want to test over here'
    ItemHeight = 13
    ParentShowHint = False
    ShowHint = True
    Sorted = True
    TabOrder = 1
    OnDragDrop = DSNListBoxDragDrop
    OnDragOver = ListBox2DragOver
  end
  object BitBtn1: TBitBtn
    Left = 72
    Top = 336
    Width = 75
    Height = 25
    TabOrder = 2
    Kind = bkCancel
  end
  object BitBtn2: TBitBtn
    Left = 240
    Top = 336
    Width = 75
    Height = 25
    Enabled = False
    TabOrder = 3
    Kind = bkOK
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 365
    Width = 388
    Height = 19
    Panels = <>
    SimplePanel = False
  end
end
