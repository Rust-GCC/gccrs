      >>PUSH SOURCE FORMAT
      >>SOURCE FIXED
        Identification Division.
        Function-id. posix-close prototype.
        Data Division.
        Linkage Section.
          77 Return-Value Binary-Long.
          01 Lk-fd PIC 9(8) Usage COMP.
        Procedure Division using
             By Value Lk-fd
             Returning Return-Value.
        End Function posix-close.
      >>POP SOURCE FORMAT
