       *> { dg-do run }
       *> { dg-options "-dialect mf" }
       *> { dg-output-file "group2/CBL_ALLOC_MEM___CBL_FREE_MEM.out" }

        identification division.
        program-id. uat_cbl_alloc_mem.

        environment division.
        configuration section.
          source-computer. Posix
        >>if debugging-mode is defined
          with debugging mode
        >>end-if
        .
          object-computer. Posix.

        data division.
        working-storage section.
        01  mem-pointer     usage is pointer.
        01  mem-size        pic x(8) comp-5 value 1.
      *> According to
      *> https://docs.rocketsoftware.com/bundle/visualcoboleclux_ug_110/page/pvv1742952958145.html
      *>
      *> flags:
      *>  bit0: shared memory
      *>  bit1: reserved (zero)
      *>  bit2: program-independent (what does that mean?)
      *>  bit3: thread-local storage
      *>  bitn: reserved (zero)
      *>
      *> Unfortunately, it is not possible to ensure whether
      *> CBL_ALLOC_MEM will honor the semantics behind these flags
      *> without looking at its implementation, so just keep them
      *> assigned to zeros.
        01  flags           pic x(8) comp-5 value 0.
        01  status-code     pic x(2) comp-5.

        procedure division.
            call "CBL_ALLOC_MEM" using mem-pointer
                                 by value mem-size
                                 by value flags
                                 returning status-code.
            if status-code is not equal to 0
                display "CBL_ALLOC_MEM failed with " status-code
            else
                display "CBL_ALLOC_MEM was sucessful"
                call "CBL_FREE_MEM" using by value mem-pointer
                                    returning status-code

                if status-code is not equal to 0
                  display "CBL_FREE_MEM failed with " status-code
                else
                  display "CBL_FREE_MEM was sucessful"
                end-if
            end-if.
            goback.

