       *> { dg-do run }
       *> { dg-output-file "group2/FUNCTION_TRIM_with_NATIONAL_characters.out" }
        identification          division.
        program-id.             prog.
        environment             division.
        configuration           section.
        special-names.
            locale cp1252   is "cp1252"
            locale utf16le  is "utf16le".
         Object-Computer.
             linux
             Character Classification
                 for Alphanumeric is cp1252
                 for National     is utf16le.
        data                    division.
        working-storage         section.
        01 from-8   pic x(10) value  "   8888   ".
        01 from-16  pic n(10) value N"   1616   ".
        01 to-8     pic x(10).
        01 to-16    pic n(10).
        procedure               division.
            display "Alphanumeric literal " function byte-length(from-8)
            display """" from-8  """"
            display """" function trim(from-8 leading )              """"
            display """" function trim(from-8 trailing )             """"
            display """" function trim(from-8)                       """"

            display "National literal " function byte-length(from-16)
            display """" from-16 """"
            display """" function trim(from-16 leading )""""
            display """" function trim(from-16 trailing )""""
            display """" function trim(from-16)""""

            display "Alphanumeric data item " function byte-length(from-8)
            move from-8                to to-8
            display """" to-8 """"
            move function trim(from-8 leading)  to to-8 
            display """" to-8 """"
            move function trim(from-8 trailing) to to-8 
            display """" to-8 """"
            move function trim(from-8)          to to-8 
            display """" to-8 """"

            display "National data item " function byte-length(from-16)
            move from-16                to to-16
            display """" to-16 """"
            move function trim(from-16 leading)  to to-16 
            display """" to-16 """"
            move function trim(from-16 trailing) to to-16 
            display """" to-16 """"
            move function trim(from-16)          to to-16 
            display """" to-16 """"
            goback.
        end program             prog.

