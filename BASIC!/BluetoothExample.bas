Rem Beck 6/1/18: Basic! Bluetooth example
BT.open

BT.status s
If s = 3 Then

    bt.read.ready rr
    if rr
        accum$=""

        do
            bt.read.bytes resp$
            accum$ = accum$ + resp$
            bt.read.ready rr
        until rr = 0

        datlen = len(accum$)

        for i = 1 to datlen

            Sw.begin mid$(accum$,i,1)

            Sw.case chr$(10)
                GoSub Assign
                Sw.break
            Sw.case chr$(13)
                gosub Assign
                Sw.break
            Sw.case "P"
                slct = 1
                Sw.break
            Sw.case "I"
                slct = 2
                Sw.break
            Sw.case "O"
                slct = 3
                Sw.break
            Sw.case "-"
                target$ = "-"
                Sw.break
            Sw.case "0"
                target$ = target$ + "0"
                Sw.break
            Sw.case "1"
                target$ = target$ + "1"
                Sw.break
            Sw.case "2"
                target$ = target$ + "2"
                Sw.break
            Sw.case "3"
                target$ = target$ + "3"
                Sw.break
            Sw.case "4"
                target$ = target$ + "4"
                Sw.break
            Sw.case "5"
                target$ = target$ + "5"
                Sw.break
            Sw.case "6"
                target$ = target$ + "6"
                Sw.break
            Sw.case "7"
                target$ = target$ + "7"
                Sw.break
            Sw.case "8"
                target$ = target$ + "8"
                Sw.break
            Sw.case "9"
                target$ = target$ + "9"
                Sw.break
            Sw.case "?"
                slct = 100
                Sw.break
            Sw.case ":"
                slct = 101
                Sw.break

            Sw.default

            Sw.end

        next
    Else
        Gosub SendData
    EndIf
Else
