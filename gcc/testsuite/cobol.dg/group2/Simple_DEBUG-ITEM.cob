       *> { dg-do run }
       *> { dg-output-file "group2/Simple_DEBUG-ITEM.out" }
        identification          division.
        program-id.             prog.
        data                    division.
        working-storage         section.
        procedure               division.
            perform debugs
            move    "6chars" to debug-line
            move    "30chars======================>" to debug-name
            move 1234 to debug-sub-1
            move -4321 to debug-sub-2
            move 9876 to debug-sub-3
            move all 'A' to debug-contents
            perform debugs
            goback.
        debugs.
            display "DEBUG-ITEM     " """" DEBUG-ITEM    """" 
            display "DEBUG-LINE     " """" DEBUG-LINE    """" 
            display "DEBUG-NAME     " """" DEBUG-NAME    """" 
            display "DEBUG-SUB-1    " """" DEBUG-SUB-1   """" 
            display "DEBUG-SUB-2    " """" DEBUG-SUB-2   """" 
            display "DEBUG-SUB-3    " """" DEBUG-SUB-3   """" 
            display "DEBUG-CONTENTS " """" DEBUG-CONTENTS"""" 
            continue.
        end program             prog.

