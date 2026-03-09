       *> { dg-do run }
       *> { dg-output-file "group2/ALLOCATE_Rules_6_-_9._Without_OPTION_INITIALIZE_Without_-fdefaultbyte___ASCII_.out" }

        identification          division.
        program-id.             prog.
        *> options. initialize working-storage X"35".
        data                    division.
        working-storage         section.
        01   based-var          based.
         02  based-x            pic x(24) value "I am I, Don Quixote".
         02  based-9            pic 999   value 123.
         02  based-p            pointer   value NULL.
        01   allocated-pointer  pointer.
         
        procedure division.
        *> Do a sanity check of the FREE operation:
        allocate    based-var
        free        based-var
        if address of based-var not equal NULL
            display "based-var should be NULL"
            end-if
        if address of based-x not equal NULL
            display "based-x should be NULL"
            end-if
        if address of based-9 not equal NULL
            display "based-9 should be NULL"
            end-if
        if address of based-p not equal NULL
            display "based-p should be NULL"
            end-if

        display     "allocate characters INITIALIZED (ISO 2023 Rule 6: defaultbyte if specified, else zero)"
        allocate 35 characters initialized returning allocated-pointer
        set address of based-var to allocated-pointer
        perform     reportt

        display     "allocate based-var initialized (ISO 2023 Rule 7: ALL TO VALUE)"
        allocate    based-var initialized
        perform     reportt
        free        based-var

        display     "allocate characters  (ISO 2023 Rule 8: OPT_INIT if specified, otherwise defaultbyte, otherwise zero)"
        allocate 35 characters returning allocated-pointer
        set address of based-var to allocated-pointer
        perform     reportt
        free        allocated-pointer

        display     "allocate based-var (ISO 2023 Rule 9: pointers NULL, otherwise OPT_INIT)"
        allocate    based-var 
        perform     reportt
        free        based-var

        goback.

        reportt.
            display "   (1) as allocated" 
            perform reportt2
            display "   (2) after ""initialize based-var""" 
            initialize based-var
            perform reportt2
            display "   (3) after ""initialize based-var all to value""" 
            initialize based-var all to value
            perform reportt2
            continue.
        reportt2.
            display "       " """" based-x """" with no advancing
            display space     """" based-9 """" with no advancing
            display space       based-p.
            continue.
        end program             prog.

