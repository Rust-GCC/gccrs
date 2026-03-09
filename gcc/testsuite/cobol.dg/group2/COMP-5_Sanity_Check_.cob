       *> { dg-do run }

      *> This program should produce no output.  It is a sanity check of
      *> COMP-5 moves and addition.
        program-id. comp5.
        data division.
        working-storage section.
        77 var PIC 999V999 COMP-5 .
        77 var1 PIC 999V9(1) COMP-5 .
        77 var2 PIC 999V9(2) COMP-5 .
        77 var3 PIC 999V9(3) COMP-5 .
        77 var4 PIC 999V9(4) COMP-5 .
        77 var5 PIC 999V9(5) COMP-5 .
        77 var6 PIC 999V9(6) COMP-5 .
        77 var7 PIC 999V9(7) COMP-5 .
        77 var8 PIC 999V9(8) COMP-5 .
        77 var555 PIC 999V99999999 COMP-5 VALUE 555.55555555.
        procedure division.
        move 111.111 to var.
        if var not equal to 111.111 display var.
        add 000.001 to var.
        if var not equal to 111.112 display var.
        add 000.01 to var.
        if var not equal to 111.122 display var.
        add 000.1 to var.
        if var not equal to 111.222 display var.
        add 1 to var.
        if var not equal to 112.222 display var.
        add 10 to var.
        if var not equal to 122.222 display var.
        add 100 to var.
        if var not equal to 222.222 display var.
        move 555.55555555 to var1
        move 555.55555555 to var2
        move 555.55555555 to var3
        move 555.55555555 to var4
        move 555.55555555 to var5
        move 555.55555555 to var6
        move 555.55555555 to var7
        move 555.55555555 to var8
        add 0.00000001 TO var555 giving var1 rounded
        add 0.00000001 TO var555 giving var2 rounded
        add 0.00000001 TO var555 giving var3 rounded
        add 0.00000001 TO var555 giving var4 rounded
        add 0.00000001 TO var555 giving var5 rounded
        add 0.00000001 TO var555 giving var6 rounded
        add 0.00000001 TO var555 giving var7 rounded
        add 0.00000001 TO var555 giving var8 rounded
        if var1 not equal to 555.6 display var1.
        if var2 not equal to 555.56 display var2.
        if var3 not equal to 555.556 display var3.
        if var4 not equal to 555.5556 display var4.
        if var5 not equal to 555.55556 display var5.
        if var6 not equal to 555.555556 display var6.
        if var7 not equal to 555.5555556 display var7.
        if var8 not equal to 555.55555556 display var8.
        stop run.

