PRINT "BeckBasic_PlotBluetooth.bas,6/9/18n"
!flagOri= 1    %Portrait
GoSub openScreen
GoSub DefineUserFunctions
GoSub SetupPlot
diag= diag1
GoSub PlotFrame
DataCount= 0

! Begin by opening Bluetooth
! If Bluetooth is not enabled
! the program will stop here.
!Print "Call BT.Open"
BT.Open
Pause 500 %Need pause because we're in graphics mode

! When BT is opened, the program
! will start listening for another
! device to connect. At this time
! the user can continue to wait
! for a connection are can attempt
! to connect to another device
! that is waiting for a connection

! Ask user what to do
Array.Load type$[], "Connect to listener", "Continue to listen for connection", "Quit"
title$= "Select operation mode"

! Create the menu that will be loaded
! when the screen is touched.
Array.Load menu$[], "Send", "Disconnect","Quit"

new_connection:   %Reached from GoTo about 60 lines down
xdomenu= 0
Select type, type$[], title$

! If the user pressed the back
! key or selected quit then quit
! otherwise try to connect to
! a listener
IF (type = 0) | (type =3)
 BT.CLOSE
 End "END: See you later"
ELSEIF type = 1
 BT.CONNECT
ENDIF

! Read status until a connection is made
ln= 0
DO
 BT.STATUS s
 IF s = 1
  ln = ln + 1
  Print "Listening", ln
 ELSEIF s =2
  Print "Connecting"
 ELSEIF s = 3
  Print "Connected"
  Print "Touch any text line to send, disconnect or quit."
 ENDIF    %IF s=1
 Pause 1000
UNTIL s = 3

! When a connection is made
! get the name of the connected
! device
BT.DEVICE.NAME device$

! *** Read/Write Loop ****
RW_Loop:
DO
 ! If the screen is touched, the interrupt
 ! code will change xdoMemu to 1 (true)
 ! In that case, show the menu
 IF xdoMenu
  xdoMenu= 0
  SELECT menu,  menu$[], "Do what?"
  IF menu = 1 THEN GOSUB xdoSend
  IF menu = 2 THEN BT.DISCONNECT
  IF menu = 3
   BT.CLOSE
   End "END: Bye bye"
  ENDIF   %IF menu=3
 ENDIF  %IF xdoMenu

 ! Read status to insure
 ! that we remain connected.
 ! If disconnected, program
 ! reverts to listen mode.
 ! In that case, ask user
 ! what to do.

 BT.STATUS s
 If s <> 3
  Print "Connection lost"
  GOTO new_connection
 Endif    %If s<>3

 ! Read messages until
 ! the message queue is
 ! empty
 Do
  BT.READ.READY rr
  If rr
   BT.READ.BYTES PedalForceStr$
   GoSub PlotDataValue
  EndIf   %If rr
 Until (rr = 0)   %BT.READ.READY
Until 0   %RW_Loop


PlotDataValue:
  ! Print "PlotDataValue: Received "; PedalForceStr$; " from "; device$
  DataCount= DataCount + 1
  If Is_Number(PedalForceStr$) Then
    ! Print "PlotDataValue: PedalPedalForceStr$ Length=  "; Len(PedalForceStr$)
    ! Print "PlotDataValue: Call Val() "
    PedalForce= VAL(PedalForceStr$)
    ! Print "PlotDataValue: Converted to "; PedalForce
  Else
    PRINT "PlotDataValue: ERROR: "; PedalForceStr$; " is not a number"
    Return
  EndIf  %If Is_Number(PedalForceStr$)
  ValPixX= PixXLeft + (DataCount / RangeX) * PixX
  ValPixY= PixYBot  - (PedalForce/ RangeY) * PixY
  Call SetColor(lineCol$, curFill)
  GR.Set.Stroke  linewidth
  GR.Line  LastObj, LastValPixX, LastValPixY, ValPixX, ValPixY
  LastValPixX= ValPixX
  LastValPixY= ValPixY
  GR.Render
Return  %PlotDataValue


! Get and send message
! to the connected device
xdoSend:
  INPUT "Text to send", wmsg$
  BT.WRITE wmsg$
  Print "Me: "; wmsg$
Return  %xdoSend


! When Console is touched
! set xdoMenu to true
onConsoleTouch:
  xdoMenu = 1
ConsoleTouch.Resume

!*************************************************************8
! Code fromBeckBasic_PlotFunc.bas,6/3/18m
SetupPlot:    %GoSub label
  Print "SetupPlot: Begin"
  BUNDLE.CREATE     diag1
  BUNDLE.PUT        diag1, "npoints"      ,  ctr
  BUNDLE.PUT        diag1, "xs"           ,   0   %Xstart
  ! BUNDLE.PUT        diag1, "xe"           ,  20   %Xend
  BUNDLE.PUT        diag1, "xe"           ,  200   %Xend
  BUNDLE.PUT        diag1, "ys"           , -20   %Ybot
  BUNDLE.PUT        diag1, "ye"           ,  20   %Ytop
  ! BUNDLE.PUT        diag1, "posX1"        ,  0.05
  ! BUNDLE.PUT        diag1, "posY1"        ,  0.05
  ! BUNDLE.PUT        diag1, "posX2"        ,  0.95
  ! BUNDLE.PUT        diag1, "posY2"        ,  0.3
  BUNDLE.PUT        diag1, "posX1"        ,  0.00
  BUNDLE.PUT        diag1, "posY1"        ,  0.00
  BUNDLE.PUT        diag1, "posX2"        ,  1.00
  If (Orientation = 1) Then     %Portrait
    BUNDLE.PUT        diag1, "posY2"        ,  0.30
  Else
    BUNDLE.PUT        diag1, "posY2"        ,  1.00
  EndIf
  BUNDLE.PUT        diag1, "cntDivX"      ,  10
  BUNDLE.PUT        diag1, "cntDivY"      ,  8
  BUNDLE.PUT        diag1, "nDigitXaxis"  ,  0
  BUNDLE.PUT        diag1, "nDigitYaxis"  ,  1
  ! BUNDLE.PUT        diag1, "border"       ,  0.09
  BUNDLE.PUT        diag1, "BorderLeft"   ,  0.05
  BUNDLE.PUT        diag1, "BorderRight"  ,  0.01
  BUNDLE.PUT        diag1, "BorderBot"    ,  0.05
  BUNDLE.PUT        diag1, "BorderTop"    ,  0.05
  BUNDLE.PUT        diag1, "borderCol"    ,  " 250 200 200 200 "
  BUNDLE.PUT        diag1, "backGrCol"    ,  " 255  58  58  58  "
  BUNDLE.PUT        diag1, "gridCol"      ,  " 255  50 100  50  "
  BUNDLE.PUT        diag1, "lineCol"      ,  " 190 200 200  20  "
  BUNDLE.PUT        diag1, "lineWidth"    ,  2
  !BUNDLE.PUT        diag1, "numbersSize"  ,  16
  BUNDLE.PUT        diag1, "numbersSize"  ,  30
  BUNDLE.PUT        diag1, "updaterate"   ,  20
  BUNDLE.PUT        diag1, "useMarker"    ,  0
  BUNDLE.PUT        diag1, "markerSize"   ,  2
  !BUNDLE.PUT        diag1, "useFillArea"  ,  1
  BUNDLE.PUT        diag1, "useFillArea"  ,  0
  BUNDLE.PUT        diag1, "alphaFillArea",  50
  BUNDLE.PUT        diag1, "useTopAxis"   ,  0
  BUNDLE.PUT        diag1, "useRightAxis" ,  0
Return  %SetupPlot


PlotFrame:    %GoSub label
  Print "PlotFrame: Begin"
  BUNDLE.GET        diag, "npoints"     ,  npoints
  BUNDLE.GET        diag, "xs"          ,  xs
  BUNDLE.GET        diag, "xe"          ,  xe
  BUNDLE.GET        diag, "ys"          ,  ys
  BUNDLE.GET        diag, "ye"          ,  ye
  BUNDLE.GET        diag, "posX1"       ,  posX1
  BUNDLE.GET        diag, "posY1"       ,  posY1
  BUNDLE.GET        diag, "posX2"       ,  posX2
  BUNDLE.GET        diag, "posY2"       ,  posY2
  BUNDLE.GET        diag, "cntDivX"     ,  cntDivX
  BUNDLE.GET        diag, "cntDivY"     ,  cntDivY
  BUNDLE.GET        diag, "nDigitXaxis" ,  nDigitXaxis
  BUNDLE.GET        diag, "nDigitYaxis" ,  nDigitYaxis
  ! BUNDLE.GET        diag, "border"      ,  border
  BUNDLE.GET        diag, "BorderLeft"  ,  BorderLeft
  BUNDLE.GET        diag, "BorderRight" ,  BorderRight
  BUNDLE.GET        diag, "BorderBot"   ,  BorderBot
  BUNDLE.GET        diag, "BorderTop"   ,  BorderTop
  BUNDLE.GET        diag, "borderCol"   ,  borderCol$
  BUNDLE.GET        diag, "backGrCol"   ,  backGrCol$
  BUNDLE.GET        diag, "gridCol"     ,  gridCol$
  BUNDLE.GET        diag, "lineCol"     ,  lineCol$
  BUNDLE.GET        diag, "lineWidth"   ,  lineWidth
  BUNDLE.GET        diag, "numbersSize" ,  numbersSize
  BUNDLE.GET        diag, "updaterate"  ,  updaterate
  BUNDLE.GET        diag, "useMarker"   ,  useMarker
  BUNDLE.GET        diag, "markerSize"  ,  markerSize
  BUNDLE.GET        diag, "useFillArea" ,  useFillArea
  BUNDLE.GET        diag, "alphaFillArea", alphaFillArea
  BUNDLE.GET        diag, "useTopAxis"  ,  useTopAxis
  BUNDLE.GET        diag, "useRightAxis",  useRightAxis

  GR.Screen curW, curH
  Print "PlotFrame: GR.Screen returned curW= "; curW; ", curH= "; curH
  !Note4 Portrait 1440 x 2560
  
  offsX          =  posX1 * curW
  offsY          =  posY1 * curH
  widX           =  ( posX2 - posX1 ) * curW
  widY           =  ( posY2 - posY1 ) * curH
  ! borderX        =  border * widX
  ! borderY        =  border * widY
  ! pixX           =  widX - (2 * borderX)
  ! pixY           =  widY - (2 * borderY)
  BorderLeftX    =  BorderLeft * widX
  BorderRightX   =  BorderRight * widX
  BorderBotY     =  BorderBot * widX
  BorderTopY     =  BorderTop * widX
  PixX           =  widX - (BorderLeftX + BorderRightX)
  PixY           =  widY - (BorderBotY  + BorderTopY)

  RangeX= xe - xs
  RangeY= ye - ys
  ! PixXLeft= offsX + borderX
  ! PixYBot = offsY + borderY + PixY 
  PixXLeft= offsX + BorderLeftX
  PixYBot = offsY + BorderTopY + PixY 
  LastValPixX= PixXLeft
  LastValPixY= PixYBot

  fmt$           =  "################"          %That's (16) "#"s
  !pScax          =  pixX / (xe-xs)
  !pScay          =  pixY / (ye-ys)
  !border         =  (border * ((pixX + pixy)/2))/2

  curCol$= borderCol$
  curFill= 1
  !Print "PlotFrame(): Call SetColor("; curCol$, curFill; ")"
  Call SetColor(curCol$, curFill)
  !Print "PlotFrame: Call GR.Rect"
  GR.Rect nn, offsX, offsY, offsX + widx, offsY + widy

  curCol$= backGrCol$
  curFill= 1
  !Print "PlotFrame: Call SetColor()"
  Call SetColor(curCol$, curFill)
  !Print "PlotFrame(): Call GR.Rect"
  ! GR.Rect nn, (offsX + borderx), (offsY + bordery), (offsX + pixX + borderx), (offsY + pixY + bordery)
  GR.Rect nn, (offsX + BorderLeftX),         (offsY + BorderTopY), ~
              (offsX + pixX + BorderRightX), (offsY + pixY + BorderBotY)

  !**** Draw Grid ****
  curCol$= gridCol$
  Call SetColor(curCol$, curFill)

  GR.SET.STROKE     1
  GR.TEXT.SIZE      numbersSize
  GR.TEXT.ALIGN     2
  ! Vertical grid lines and X axis labels
  For i= 0 To cntDivX
    ! Xstart= (offsX + borderx + i*(pixX / cntDivX))
    Xstart= (offsX + BorderLeftX + i*(pixX / cntDivX))
    Xend  = Xstart
    ! Ystart= (offsY + bordery)
    ! Yend  = (offsY + bordery + pixY)
    Ystart= (offsY + BorderTopY)
    Yend  = (offsY + BorderTopY + pixY)
    !Print "PlotFrame(): Before GR.Line call, Xpos, Ystart, Xpos, Yend "; Xpos, Ystart, Xpos, Yend
    GR.Line   LastObj, Xstart, Ystart, Xend, Yend
    
    fmtstr$ = "#############%." + right$(fmt$, nDigitXaxis)                         %That’s (13) “#”
    tmp$    = replace$(format$(fmtstr$, (xs + i*((xe - xs) / cntDivX))), " " , "")
    If useTopAxis Then 
      tmp1= (-numbersSize * 0.6) 
    Else 
      tmp1= (pixY + numbersSize * 1.2)
    Endif   %If useTopAxis
    !Print "PlotFrame(): Before GR.Text.Draw call, tmp$= "; tmp$
    GR.Text.Draw   LastObj, Xstart, (offsY + BorderTopY + tmp1), tmp$
  Next  %For i

 !IF useRightAxis THEN GR.TEXT.ALIGN 1 ELSE GR.TEXT.ALIGN 3
 If useRightAxis Then
   GR.TEXT.ALIGN 1 
 Else
   GR.TEXT.ALIGN 3
 Endif  %If useRightAxis
   
  ! Horizontal grid lines and Y axis labels
 For i= 0 To cntDivY
  ! Xstart= (offsX + borderx)
  ! Xend  = (offsX + pixX + borderx)
  ! Ystart= (offsY + bordery + i*(pixY / cntDivY))
  Xstart= (offsX + BorderLeftX)
  ! Xend  = (offsX + pixX + BorderLeftX)
  Xend  = (offsX + pixX)
  Ystart= (offsY + BorderTopY + i*(pixY / cntDivY))
  Yend  = Ystart
  GR.Line   LastObj, Xstart, Ystart, Xend, Yend
  
  fmtstr$= "#############%." + right$(fmt$, nDigitXaxis)
  tmp$= replace $(format$( fmtstr$, (ye - i*((ye - ys) / cntDivY))), " ", "")
  If useRightAxis Then
    tmp1= pixX + 5 
  Else
    tmp1= -5
  Endif %If useRightAxis
  GR.Text.Draw   LastObj, (offsX + borderx + tmp1), Ystart, tmp$
 Next %For i

 curCol$= lineCol$
 curFill= 1
 Call SetColor(curCol$, curFill)
 GR.SET.STROKE  linewidth
 ! xo             =  offsX+ pixX+ borderx
 ! xu             =  offsX+       borderx
 ! yo             =  offsY+ pixY+ bordery
 ! yu             =  offsY+       bordery

 curCol$= gridCol$
 GR.COLOR 255, VAL(WORD$(curCol$,2)), VAL(WORD$(curCol$,3)), VAL(WORD$(curCol$,4)), 0
 GR.SET.STROKE     linewidth+2
 ! GR.Rect           nn, offsX+borderx,offsY+bordery,offsX+pixX+borderx, offsY+pixY+bordery
 ! GR.Rect           nn, offsX + borderx, offsY + bordery,offsX + pixX + borderx, offsY + pixY + bordery
 GR.Rect  nn, (offsX + BorderLeftX),         (offsY + BorderTopY), ~
              (offsX + pixX + BorderRightX), (offsY + pixY + BorderBotY)
 GR.Render
Return  %PlotFrame


DefineUserFunctions:
FN.Def SetColor(Color$, FillNum)
  GR.Color VAL(WORD$(Color$,1)), VAL(WORD$(Color$,2)), ~
           VAL(WORD$(Color$,3)), VAL(WORD$(Color$,4)), FillNum
FN.End  %SetColor
Return  %DefineUserFunctions


!Subroutines
openScreen:
  Print "openScreen: Begin"
  !refW       = 780
  !refH       = 1280
  !IF !flagOri  THEN SWAP refW, refH
  !centW      = refW/2
  !centH      = refH/2
  !GR.OPEN      255,0,0,20,0,flagOri
  Alpha         = 255     %Transparancy
  Red           = 0
  Green         = 0
  Blue          = 20
  ShowStatusBar = 0
  Orientation   = 1      %Landscape: 0, Portrait: 1
  !GR.Open      255, 0, 0, 20, ShowStatusBar, Orientation   % Alpha, Red, Green, Blue, ShowStatusBar, Orientation
  GR.Open      Alpha, Red, Green, Blue, ShowStatusBar, Orientation  
  GR.Screen    curW, curH
  Print "openScreen: GR.Screen returned curW= "; curW; ", curH= "; curH
  !Samsung Note4 Landscape: 2560 x 1440 Portrait: 1440 x 2560
  !scaW       = curW / refW
  !scaH       = curH / refH
  !GR.SCALE     scaW , scaH
  desLoopTime= 50
Return  %openScreen

! onError:
! Print "onError: Begin"
! Print GetError$() 
! End "END: onError:"
!Last line.
