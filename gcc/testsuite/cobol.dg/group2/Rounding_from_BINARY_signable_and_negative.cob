       *> { dg-do run }
       *> { dg-output-file "group2/Rounding_from_BINARY_signable_and_negative.out" }
        identification              division.
        program-id.                 prog.
        data                        division.
        working-storage             section.
        77 v0 pic s9999v9 binary value -1234.0 .
        77 v1 pic s9999v9 binary value -1234.1 .
        77 v5 pic s9999v9 binary value -1234.5 .
        77 v9 pic s9999v9 binary value -1234.9 .
        77 d0   pic s9999   display             .
        77 d1   pic s9999   display             .
        77 d5   pic s9999   display             .
        77 d9   pic s9999   display             .

        procedure                   division.
            display "-1234.0 -1234.1 -1234.5 -1234.9"
            compute d0                     = v0
            compute d1                     = v1
            compute d5                     = v5
            compute d9                     = v9
            display d0 "   " d1 "   " d5 "   " d9 "   " "truncated"

            compute d0 ROUNDED MODE AWAY-FROM-ZERO = v0
            compute d1 ROUNDED MODE AWAY-FROM-ZERO = v1
            compute d5 ROUNDED MODE AWAY-FROM-ZERO = v5
            compute d9 ROUNDED MODE AWAY-FROM-ZERO = v9
            display d0 "   " d1 "   " d5 "   " d9 "   " "away-from-zero"

            compute d0 ROUNDED MODE NEAREST-AWAY-FROM-ZERO = v0
            compute d1 ROUNDED MODE NEAREST-AWAY-FROM-ZERO = v1
            compute d5 ROUNDED MODE NEAREST-AWAY-FROM-ZERO = v5
            compute d9 ROUNDED MODE NEAREST-AWAY-FROM-ZERO = v9
            display d0 "   " d1 "   " d5 "   " d9 "   " "nearest-away-from-zero"

            compute d0 ROUNDED MODE NEAREST-EVEN = v0
            compute d1 ROUNDED MODE NEAREST-EVEN = v1
            compute d5 ROUNDED MODE NEAREST-EVEN = v5
            compute d9 ROUNDED MODE NEAREST-EVEN = v9
            display d0 "   " d1 "   " d5 "   " d9 "   " "nearest-even"

            compute d0 ROUNDED MODE NEAREST-TOWARD-ZERO = v0
            compute d1 ROUNDED MODE NEAREST-TOWARD-ZERO = v1
            compute d5 ROUNDED MODE NEAREST-TOWARD-ZERO = v5
            compute d9 ROUNDED MODE NEAREST-TOWARD-ZERO = v9
            display d0 "   " d1 "   " d5 "   " d9 "   " "nearest-toward-zero"

            compute d0 ROUNDED MODE TOWARD-GREATER = v0
            compute d1 ROUNDED MODE TOWARD-GREATER = v1
            compute d5 ROUNDED MODE TOWARD-GREATER = v5
            compute d9 ROUNDED MODE TOWARD-GREATER = v9
            display d0 "   " d1 "   " d5 "   " d9 "   " "toward-greater"

            compute d0 ROUNDED MODE TOWARD-LESSER = v0
            compute d1 ROUNDED MODE TOWARD-LESSER = v1
            compute d5 ROUNDED MODE TOWARD-LESSER = v5
            compute d9 ROUNDED MODE TOWARD-LESSER = v9
            display d0 "   " d1 "   " d5 "   " d9 "   " "toward-lesser"

            goback.
        end program                 prog.

