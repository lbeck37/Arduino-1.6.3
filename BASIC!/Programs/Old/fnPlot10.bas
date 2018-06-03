flagOri     = 0
GOSUB openScreen
GOSUB userfunctions


! test it --------------------------------------------


! create some functions ----------------------

_2pi            = 2*3.14159
aa              = 600
DIM               x  [ aa ]
DIM               y1 [ aa ]
DIM               y2 [ aa ]
DIM               y3 [ aa ]
DIM               y4 [ aa ]
FOR i            =  -_2pi TO _2pi STEP  0.10
 ctr             += 1
 x  [ctr]        =  i
 y1 [ctr]        =  sin (4*i)
 IF                 y1[ctr] >=  0.8 THEN y2 [ctr] = 0.85
 IF                 y1[ctr] <= -0.8 THEN y2 [ctr] = -0.85
 y3 [ctr]        =  sin (2*i)
 y4 [ctr]        =  sin (1*i)+(RND()-0.5)
NEXT



! create a diagram bundle (...object) -------

BUNDLE.CREATE     diag1

BUNDLE.PUT        diag1, "npoints"      ,  ctr
BUNDLE.PUT        diag1, "xs"           , -_2pi
BUNDLE.PUT        diag1, "xe"           ,  _2pi
BUNDLE.PUT        diag1, "ys"           ,  -1.1
BUNDLE.PUT        diag1, "ye"           ,  1.1
BUNDLE.PUT        diag1, "posX1"        ,  0.05
BUNDLE.PUT        diag1, "posY1"        ,  0.05
BUNDLE.PUT        diag1, "posX2"        ,  0.95
BUNDLE.PUT        diag1, "posY2"        ,  0.5
BUNDLE.PUT        diag1, "cntDivX"      ,  12
BUNDLE.PUT        diag1, "cntDivY"      ,  9
BUNDLE.PUT        diag1, "border"       ,  0.09
BUNDLE.PUT        diag1, "borderCol"    ,  " 250 200 200 200 "
BUNDLE.PUT        diag1, "backGrCol"    ,  " 255 58  58  58  "
BUNDLE.PUT        diag1, "gridCol"      ,  " 255 50  100 50  "
BUNDLE.PUT        diag1, "lineCol"      ,  " 190 200 200  20  "
BUNDLE.PUT        diag1, "lineWidth"    ,  2
BUNDLE.PUT        diag1, "numbersSize"  ,  16
BUNDLE.PUT        diag1, "nDigitXaxis"  ,  2
BUNDLE.PUT        diag1, "nDigitYaxis"  ,  4
BUNDLE.PUT        diag1, "updaterate"   ,  20
BUNDLE.PUT        diag1, "useMarker"    ,  0
BUNDLE.PUT        diag1, "markerSize"   ,  2
BUNDLE.PUT        diag1, "useFillArea"  ,  1
BUNDLE.PUT        diag1, "alphaFillArea",  50
BUNDLE.PUT        diag1, "useTopAxis"   ,  0
BUNDLE.PUT        diag1, "useRightAxis" ,  0

! plot it -------

CALL              plot (diag1, x [], y1 [])


! make a copy, modify it a bit,  plot it -------
! plot it as a 2nd layer by setting alpha-values=0 !! ----

diag2           = bundleCopy( diag1 )
BUNDLE.PUT        diag2, "lineCol"     ,  " 190 200  0  20  "
BUNDLE.PUT        diag2, "borderCol"   ,  " 0 200 200 200 "
BUNDLE.PUT        diag2, "backGrCol"   ,  " 0 58  58  58  "
BUNDLE.PUT        diag2, "gridCol"     ,  " 0 50  120 50  "
BUNDLE.PUT        diag2, "useFillArea" ,  0
BUNDLE.PUT        diag2, "lineWidth"   ,  2
CALL              plot (diag2, x [], y2 [])


! do it again ----------

diag3           = bundleCopy( diag1 )
BUNDLE.PUT        diag3, "posX1"        ,  0.05
BUNDLE.PUT        diag3, "posY1"        ,  0.50
BUNDLE.PUT        diag3, "posX2"        ,  0.95
BUNDLE.PUT        diag3, "posY2"        ,  0.95
BUNDLE.PUT        diag3, "lineCol"      ,  " 160 20 200  200  "
BUNDLE.PUT        diag3, "useFillArea"  ,  0
BUNDLE.PUT        diag3, "lineWidth"    ,  2
BUNDLE.PUT        diag3, "useMarker"    ,  1
CALL              plot (diag3, x[], y3 [])


! brief stop ----------

POPUP "brief stop....",0,0,0
PAUSE  3000



! re-arrange the three diags  ------
GR.CLS
diag            = diag1
BUNDLE.PUT        diag, "posX1"        ,  0.05
BUNDLE.PUT        diag, "posY1"        ,  0.05
BUNDLE.PUT        diag, "posX2"        ,  0.498
BUNDLE.PUT        diag, "posY2"        ,  0.498
BUNDLE.PUT        diag, "useTopAxis"   ,  1
BUNDLE.PUT        diag, "borderCol"    ,  " 0 200 200 200 "
BUNDLE.PUT        diag, "border"       ,  0.02

diag            = diag2
BUNDLE.PUT        diag, "posX1"        ,  0.502
BUNDLE.PUT        diag, "posY1"        ,  0.05
BUNDLE.PUT        diag, "posX2"        ,  0.95
BUNDLE.PUT        diag, "posY2"        ,  0.498
BUNDLE.PUT        diag, "borderCol"    ,  " 0 200 200 200 "
BUNDLE.PUT        diag, "backGrCol"    ,  " 255 58  58  58  "
BUNDLE.PUT        diag, "gridCol"      ,  " 255 50  120 50  "
BUNDLE.PUT        diag, "useTopAxis"   ,  1
BUNDLE.PUT        diag, "useRightAxis" ,  1
BUNDLE.PUT        diag, "border"       ,  0.02
BUNDLE.PUT        diag, "useMarker"    ,  1

diag            = diag3
BUNDLE.PUT        diag, "posX1"        ,  0.05
BUNDLE.PUT        diag, "posY1"        ,  0.502
BUNDLE.PUT        diag, "posX2"        ,  0.498
BUNDLE.PUT        diag, "posY2"        ,  0.95
BUNDLE.PUT        diag, "borderCol"    ,  " 0 200 200 200 "
BUNDLE.PUT        diag, "border"       ,  0.02
CALL              plot (diag1, x [], y1 [])
CALL              plot (diag2, x [], y2 [])
CALL              plot (diag3, x [], y3 [])


! add another one ----------

diag4           = bundleCopy( diag1 )
diag            = diag4
BUNDLE.PUT        diag, "posX1"        ,  0.502
BUNDLE.PUT        diag, "posY1"        ,  0.502
BUNDLE.PUT        diag, "posX2"        ,  0.95
BUNDLE.PUT        diag, "posY2"        ,  0.95
BUNDLE.PUT        diag, "lineCol"      ,  " 155 50  12 250  "
BUNDLE.PUT        diag, "borderCol"    ,  " 0 200 200 200 "
BUNDLE.PUT        diag, "useTopAxis"   ,  0
BUNDLE.PUT        diag, "useRightAxis" ,  1
BUNDLE.PUT        diag, "border"       ,  0.02
CALL              plot (diag4, x [], y4 [])










DO
UNTIL0


END



!--------------------------------------------
userfunctions:

FN.DEF             bundleCopy( bundleIn )
 BUNDLE.CREATE     bundleOut
 BUNDLE.KEYS       bundleIn , keys
 LIST.SIZE         keys, len
 FOR i             =1 TO len
  LIST.GET         keys, i, key$
  BUNDLE.TYPE      bundleIn, key$,  type$
  IF               type$ ="N"
   BUNDLE.GET      bundleIn , key$, tmp
   BUNDLE.PUT      bundleOut, key$, tmp
  ELSE
   BUNDLE.GET      bundleIn , key$, tmp$
   BUNDLE.PUT      bundleOut, key$, tmp$
  ENDIF
 NEXT
 FN.RTN            bundleOut
FN.END



FN.DEF             plot (diag, xval [], yval [])

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
 BUNDLE.GET        diag, "border"      ,  border
 BUNDLE.GET        diag, "borderCol"   ,  borderCol$
 BUNDLE.GET        diag, "backGrCol"   ,  backGrCol$
 BUNDLE.GET        diag, "gridCol"     ,  gridCol$
 BUNDLE.GET        diag, "lineCol"     ,  lineCol$
 BUNDLE.GET        diag, "lineWidth"   ,  lineWidth
 BUNDLE.GET        diag, "numbersSize" ,  numbersSize
 BUNDLE.GET        diag, "nDigitXaxis" ,  nDigitXaxis
 BUNDLE.GET        diag, "nDigitYaxis" ,  nDigitYaxis
 BUNDLE.GET        diag, "updaterate"  ,  updaterate
 BUNDLE.GET        diag, "useMarker"   ,  useMarker
 BUNDLE.GET        diag, "markerSize"  ,  markerSize
 BUNDLE.GET        diag, "useFillArea" ,  useFillArea
 BUNDLE.GET        diag, "alphaFillArea", alphaFillArea
 BUNDLE.GET        diag, "useTopAxis"  ,  useTopAxis
 BUNDLE.GET        diag, "useRightAxis",  useRightAxis

 GR.SCREEN         curW, curH
 offsX          =  posX1 * curW
 offsY          =  posY1 * curH
 widX           =  ( posX2 - posX1 ) * curW
 widY           =  ( posY2 - posY1 ) * curH
 borderX        =  border * widX
 borderY        =  border * widY
 pixX           =  widX - 2* borderX
 pixY           =  widY - 2* borderY

 fmt $          =  "################"
 pScax          =  pixX/(xe-xs)
 pScay          =  pixY/(ye-ys)
 border         =  (border *(pixX+pixy)/2)/2



 curCol$         = borderCol$
 curFill         = 1
 GOSUB             setColor
 GR.RECT           nn,  offsX , offsY , offsX+ widx , offsY+ widy


 curCol$         = backGrCol$
 curFill         = 1
 GOSUB setColor
 GR.RECT           nn, offsX+borderx,offsY+bordery,offsX+pixX+borderx, offsY+pixY+bordery

 curCol$         = gridCol$
 GOSUB             setColor

 GR.SET.STROKE     1
 GR.TEXT.SIZE      numbersSize


 GR.TEXT.ALIGN     2
 FOR i           = 0 TO cntDivX
  tmp            = offsX+ borderx + i*pixX/cntDivX
  fmtstr$        = "#############%."+right $(fmt$, nDigitXaxis)
  tmp$           = replace $(format$ ( fmtstr$, xs+i*(xe-xs)/cntDivX)," " ,"")
  GR.LINE          nn,  tmp, offsY+ bordery, tmp, offsY+ pixY+ bordery
  IF               useTopAxis THEN tmp1= -numbersSize*0.6 ELSE tmp1= pixY+ numbersSize*1.2
  GR.TEXT.DRAW     nn,  tmp, offsY+ bordery+ tmp1 , tmp$
 NEXT

 IF                useRightAxis THEN GR.TEXT.ALIGN 1 ELSE GR.TEXT.ALIGN 3
 FOR i           = 0 TO cntDivY
  tmp            = offsY+ bordery + i*pixY/cntDivY
  fmtstr$        = "#############%."+right $(fmt$, nDigitXaxis)
  tmp$           = replace $(format$ ( fmtstr$, ye - i* (ye-ys) /cntDivY )," " ,"")
  GR.LINE          nn, offsX+ borderx, tmp, offsX+ pixX+ borderx , tmp
  IF                useRightAxis THEN tmp1=pixX+5 ELSE tmp1=-5
  GR.TEXT.DRAW     nn, offsX+ borderx + tmp1 , tmp , tmp$
 NEXT



 curCol$         = lineCol$
 curFill         = 1
 GOSUB             setColor
 GR.SET.STROKE     linewidth
 xo             =  offsX+ pixX+ borderx
 xu             =  offsX+       borderx
 yo             =  offsY+ pixY+ bordery
 yu             =  offsY+       bordery

 DIM               xy [ npoints, 2 ]
 LIST.CREATE       n,polyplot

 FOR i          =  1 TO npoints

  xy [i,1]      =  offsX+ borderx+        (xval [i] - xs )* pScax
  xy [i,2]      =  offsY+ bordery+ pixY - (yval [i] - ys )* pScay

  IF               xy[i,1]>xo THEN xy[i,1] = xo
  IF               xy[i,1]<xu THEN xy[i,1] = xu
  IF               xy[i,2]>yo THEN xy[i,2] = yo
  IF               xy[i,2]<yu THEN xy[i,2] = yu

  IF        useMarker THEN gr.circle nn, xy [i,1], xy [i,2], markerSize
  IF               ! mod (i, updaterate) THEN gr.render

 NEXT

 LIST.ADD.ARRAY   polyplot, xy[]
 LIST.ADD         polyplot, offsX+pixX+borderx  , offsY+pixY+bordery
 LIST.ADD         polyplot, offsX+     borderx  , offsY+pixY+bordery


 ! plot polyline without filling ----
 curFill         = 0
 GOSUB            setColor
 GR.POLY          nn, polyplot

 ! plot filling ---
 IF useFillArea
  GR.COLOR  ~
  alphaFillArea        , VAL(WORD$(curCol$,2)), ~
  VAL(WORD$(curCol$,3)), VAL(WORD$(curCol$,4)), 1
  GR.SET.STROKE     3
  GR.POLY          nn, polyplot
 ENDIF


 curCol$         = gridCol$
 GR.COLOR  ~
 255                  , VAL(WORD$(curCol$,2)), ~
 VAL(WORD$(curCol$,3)), VAL(WORD$(curCol$,4)), 0
 GR.SET.STROKE     linewidth+2
 GR.RECT           nn, offsX+borderx,offsY+bordery,offsX+pixX+borderx, offsY+pixY+bordery

 GR.RENDER



 FN.RTN            1

 setColor:
 GR.COLOR  ~
 VAL(WORD$(curcol$,1)), VAL(WORD$(curCol$,2)), ~
 VAL(WORD$(curCol$,3)), VAL(WORD$(curCol$,4)), curFill
 RETURN

FN.END


RETURN
!--------------------------------------------




!--------------------------------------------
openScreen:

refW       = 780
refH       = 1280
IF !flagOri  THEN SWAP refW, refH
centW      = refW/2
centH      = refH/2
GR.OPEN      255,0,0,20,0,flagOri
GR.SCREEN    curW, curH
scaW       = curW / refW
scaH       = curH / refH
!GR.SCALE     scaW , scaH
desLoopTime= 50


RETURN
!--------------------------------------------

