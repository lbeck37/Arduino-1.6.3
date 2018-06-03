! ---------------
!  FUNCTION PLOT
!   Version 2.3
!  by luca_italy
! ---------------

CLS
CONSOLE.TITLE "Function Plot"
PAUSE 500
BUNDLE.CREATE ref1 % global bundle
BUNDLE.PUT 1,"err",0
stro=3
err_pos$="init"
GOSUB loadall

DIM main$[10]
main$[1]="f(x) = "+f$
main$[2]="x1 = "+STR$(x1)
main$[3]="x2 = "+STR$(x2)
main$[4]="dx = "+STR$(dx)
main$[5]="y1 = "+STR$(y1)
main$[6]="y2 = "+STR$(y2)
main$[7]="Stroke = "+STR$(stro)
main$[8]="Plot function"
main$[9]="Clear data"
main$[10]="Exit"
main_msg$="Function Plot - Main menu"
ARRAY.LOAD ok$[],"Ok"
wrong_msg$="Function Plot - Error: Wrong value(s)"
ARRAY.LOAD yesno$[],"Yes","No"
clr_msg$="Function Plot - Confirm to clear data?"

GOSUB saveall
DO
 SELECT choice,main$[],main_msg$
 IF choice=1 THEN GOTO newfunc
 IF choice=2 THEN
  INPUT "x1 =",x1,x1
  main$[2]="x1 = "+STR$(x1)
 ELSEIF choice=3 THEN
  INPUT "x2 =",x2,x2
  main$[3]="x2 = "+STR$(x2)
 ELSEIF choice=4 THEN
  INPUT "dx =",dx,dx
  main$[4]="dx = "+STR$(dx)
 ELSEIF choice=5 THEN
  INPUT "y1 =",y1,y1
  main$[5]="y1 = "+STR$(y1)
 ELSEIF choice=6 THEN
  INPUT "y2 =",y2,y2
  main$[6]="y2 = "+STR$(y2)
 ELSEIF choice=7 THEN
  INPUT "Stroke =",stro,stro
  main$[7]="Stroke = "+STR$(stro)
 ENDIF
 GOSUB saveall
 IF choice=8 THEN GOSUB plot
 IF choice=9 THEN
  SELECT choice1,yesno$[],clr_msg$
  IF choice1=1 THEN
   f$=""
   x1=0
   x2=0
   dx=0
   y1=0
   y2=0
   stro=3
   GOSUB saveall
   GOTO rerun
  ENDIF
 ENDIF
 IF choice=10 THEN
  CLS
  PRINT
  PRINT "Tap Back Key to exit..."
  PRINT
  GOTO finish
 ENDIF
UNTIL 0

loadall:
FILE.EXISTS bas,"function_plot.bas.txt"
FILE.EXISTS dat,"function_plot.dat.txt"
IF bas & dat THEN
 INCLUDE /../data/function_plot.bas.txt
 TEXT.OPEN r,fn,"function_plot.dat.txt"
 TEXT.READLN fn,f$
 TEXT.READLN fn,x1$
 TEXT.READLN fn,x2$
 TEXT.READLN fn,dx$
 TEXT.READLN fn,y1$
 TEXT.READLN fn,y2$
 TEXT.READLN fn,stro$
 TEXT.CLOSE fn
 x1=VAL(x1$)
 x2=VAL(x2$)
 dx=VAL(dx$)
 y1=VAL(y1$)
 y2=VAL(y2$)
 stro=VAL(stro$)
ENDIF
RETURN

newfunc:
INPUT "f(x) =",f$,f$
isin=IS_IN(CHR$(10),f$)
IF isin THEN f$=LEFT$(f$,isin-1)
GOSUB saveall
GOTO rerun

plot:
IF x1>=x2 | y1>=y2 | dx<=0 | stro<0 THEN
 SELECT choice,ok$[],wrong_msg$
 RETURN
ENDIF
POPUP "Tap the screen to go back to Main menu",0,0,0
GR.OPEN 255,255,255,102,0,0
PAUSE 1000
GR.COLOR 255,0,0,0,0
GR.SCREEN w,h
zeroy = y2*h/(y2-y1)
zerox = -x1*w/(x2-x1)
GR.LINE n,0,zeroy,w,zeroy
GR.LINE n,zerox,0,zerox,h
GR.RENDER
GR.SET.STROKE stro
GR.COLOR 255,255,0,0,0
scaley = h/(y2-y1)
scalex = w/(x2-x1)
oldx = x1
oldy = f(x1)
BUNDLE.GET 1,"err",err
BUNDLE.PUT 1,"err",0
div0x1 = 0
IF err=1 THEN div0x1=1
IF err=2 THEN GOTO rerun
FOR x=x1+dx TO x2 STEP dx
 GR.TOUCH touched,dummy,dummy
 IF touched THEN F_N.BREAK
 y = f(x)
 BUNDLE.GET 1,"err",err
 BUNDLE.PUT 1,"err",0
 IF !err THEN
  rx1 = zerox+oldx*scalex
  ry1 = zeroy-oldy*scaley
  rx2 = zerox+x*scalex
  ry2 = zeroy-y*scaley
  IF !div0x1 THEN
   GR.LINE n,rx1,ry1,rx2,ry2
  ELSE
   div0x1 = 0
  ENDIF
  GR.RENDER
  oldx = x
  oldy = y
 ENDIF
NEXT
IF !touched THEN
 DO
  GR.TOUCH touched,dummy,dummy
 UNTIL touched
ENDIF
GR.CLOSE
RETURN

saveall:
TEXT.OPEN w,fn,"function_plot.bas.txt"
TEXT.WRITELN fn,"fn.def f(x)"
TEXT.WRITELN fn,"fn.rtn "+f$
TEXT.WRITELN fn,"fn.end"
TEXT.CLOSE fn
TEXT.OPEN w,fn,"function_plot.dat.txt"
TEXT.WRITELN fn,f$
TEXT.WRITELN fn,STR$(x1)
TEXT.WRITELN fn,STR$(x2)
TEXT.WRITELN fn,STR$(dx)
TEXT.WRITELN fn,STR$(y1)
TEXT.WRITELN fn,STR$(y2)
TEXT.WRITELN fn,STR$(stro)
TEXT.CLOSE fn
RETURN

ONERROR:
IF err_pos$="run" THEN GOTO finish
IF err_pos$="" THEN
 IF LEFT$(GETERROR$(),14)="DIVIDE BY ZERO" THEN
  BUNDLE.PUT 1,"err",1
  FN.RTN 0
 ELSE
  GR.CLOSE
  err_msg$="Function Plot - Syntax error in f(x)"
  ARRAY.LOAD ok$[],"Ok"
  SELECT choice,ok$[],err_msg$
  BUNDLE.PUT 1,"err",2
  FN.RTN 0
 ENDIF
ENDIF
GOTO rerun

ONBACKKEY:
BACK.RESUME

rerun:
err_pos$="run"
RUN "function_plot.bas"

finish:

