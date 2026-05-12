       *> { dg-do run }
       *> { dg-output-file "group2/Overlapping_MOVE.out" }
        identification division.
        program-id. prog.
        data division.
        working-storage section.
        01 structure.
            05 field1 pic x(5).
            05 field2 pic x(10).
        procedure division.
            move "Hallo" to field1.
            move "1234567890" to field2.
            *> The ISO specification says specifically that when an overlapping
            *> move takes place in a single data descriptor, that it behaves
            *> like a memmove(3), and not a memcopy(3):
            move field2 to structure.
            display """" structure """"
            if field1 not = "12345"
              display "error:1: " field1
              end-display
            end-if
            if field2 not = "67890     "
              display "error:2: " field2
              end-display
            end-if
            goback.
        end program prog.

