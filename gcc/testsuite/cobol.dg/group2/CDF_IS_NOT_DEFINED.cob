       *> { dg-do run }
       *> { dg-output-file "group2/CDF_IS_NOT_DEFINED.out" }
        identification division.
        program-id. fbug.
        procedure division.

        >>IF CVAR IS NOT DEFINED
            display "case 1: correct: >>IF cvar not defined"
        >>ELSE
            display "case 1: INCORRECT: >>ELSE cvar not defined"
        >>END-IF

        >>IF CVAR IS DEFINED
            display "case 2: INCORRECT: >>IF cvar defined"
        >>ELSE
            display "case 2: correct: >>ELSE cvar defined"
        >>END-IF

        >>DEFINE CVAR AS 1

        >>IF CVAR IS NOT DEFINED
            display "case 3: INCORRECT: >>IF cvar not defined"
        >>ELSE
            display "case 3: correct: >>ELSE cvar not defined"
        >>END-IF

        >>IF CVAR IS DEFINED
            display "case 4: correct: >>IF cvar defined"
        >>ELSE
            display "case 4: INCORRECT: >>ELSE cvar defined"
        >>END-IF

            goback.

