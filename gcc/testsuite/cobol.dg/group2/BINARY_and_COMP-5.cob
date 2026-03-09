       *> { dg-do run }
       *> { dg-output-file "group2/BINARY_and_COMP-5.out" }
        identification          division.
        program-id.             prog.
        procedure               division.
        call "prog1"
        call "prog2"
        goback.
        end program             prog.

        identification          division.
        program-id.             prog1.
        data                    division.
        working-storage         section.
        01.
        02 var-binary   binary          pic  9v9(10) .
        02 var-binaryp  redefines var-binary pointer.
        02 var-comp     comp            pic  9v9(10) .
        02 var-compp    redefines       var-comp pointer.
        02 var-compu    computational   pic  9v9(10) .
        02 var-compup   redefines       var-compu pointer.
        02 var-comp4    comp-4          pic  9v9(10) .
        02 var-comp4p   redefines       var-comp4 pointer.
        02 var-compu4   computational-4 pic  9v9(10) .
        02 var-compu4p  redefines       var-compu4 pointer.

        02 var-comp5    comp-5          pic  9v9(10) .
        02 var-comp5p   redefines       var-comp5 pointer.
        02 var-compu5   computational-5 pic  9v9(10) .
        02 var-compu5p  redefines       var-compu5 pointer.

        02 var-sbinary  binary          pic s9v9(10) .
        02 var-sbinaryp redefines       var-sbinary pointer.
        02 var-scomp    comp            pic s9v9(10) .
        02 var-scompp   redefines       var-scomp pointer.
        02 var-scompu   computational   pic s9v9(10) .
        02 var-scompup  redefines       var-scompu pointer.
        02 var-scomp4   comp-4          pic s9v9(10) .
        02 var-scomp4p  redefines       var-scomp4 pointer.
        02 var-scompu4  computational-4 pic s9v9(10) .
        02 var-scompu4p redefines       var-scompu4 pointer.
        
        02 var-scomp5   comp-5          pic s9v9(10) .
        02 var-scomp5p  redefines       var-scomp5 pointer.
        02 var-scompu5  computational-5 pic s9v9(10) .
        02 var-scompu5p redefines       var-scompu5 pointer.
        procedure               division.
            move  0.0001193046 to var-binary var-comp var-compu
                                     var-comp4 var-compu4 var-comp5
                                     var-compu5
            display " " var-binary "  " var-comp "  " var-compu "  " 
                                    var-comp4 "  " var-compu4 "  " 
                                    var-comp5 "  " var-compu5
            move  0.0001193046 to var-sbinary var-scomp var-scompu
                                    var-scomp4 var-scompu4 var-scomp5
                                    var-scompu5
            display var-sbinary " " var-scomp " " var-scompu " " 
                                    var-scomp4 " " var-scompu4 " "
                                    var-scomp5 " " var-scompu5
            move -0.0001193046 to var-sbinary var-scomp var-scompu 
                                    var-scomp4 var-scompu4 var-scomp5 
                                    var-scompu5
            display var-sbinary " " var-scomp " " var-scompu " " 
                                    var-scomp4 " " var-scompu4 " " 
                                    var-scomp5 " " var-scompu5
            display var-binaryp
            display var-compp
            display var-compup
            display var-comp4p
            display var-compu4p
            display var-comp5p
            display var-compu5p

            display var-sbinaryp
            display var-scompp
            display var-scompup
            display var-scomp4p
            display var-scompu4p
            display var-scomp5p
            display var-scompu5p

            goback.
        end program             prog1.

        identification          division.
        program-id.             prog2.
        data                    division.
        working-storage         section.
        01.
        02 var-binary   pic  9v9(10) binary          .
        02 var-binaryp  redefines var-binary pointer.
        02 var-comp     pic  9v9(10) comp            .
        02 var-compp    redefines       var-comp pointer.
        02 var-compu    pic  9v9(10) computational   .
        02 var-compup   redefines       var-compu pointer.
        02 var-comp4    pic  9v9(10) comp-4          .
        02 var-comp4p   redefines       var-comp4 pointer.
        02 var-compu4   pic  9v9(10) computational-4 .
        02 var-compu4p  redefines       var-compu4 pointer.

        02 var-comp5    pic  9v9(10) comp-5          .
        02 var-comp5p   redefines       var-comp5 pointer.
        02 var-compu5   pic  9v9(10) computational-5 .
        02 var-compu5p  redefines       var-compu5 pointer.

        02 var-sbinary  pic s9v9(10) binary          .
        02 var-sbinaryp redefines       var-sbinary pointer.
        02 var-scomp    pic s9v9(10) comp            .
        02 var-scompp   redefines       var-scomp pointer.
        02 var-scompu   pic s9v9(10) computational   .
        02 var-scompup  redefines       var-scompu pointer.
        02 var-scomp4   pic s9v9(10) comp-4          .
        02 var-scomp4p  redefines       var-scomp4 pointer.
        02 var-scompu4  pic s9v9(10) computational-4 .
        02 var-scompu4p redefines       var-scompu4 pointer.
        
        02 var-scomp5   pic s9v9(10) comp-5          .
        02 var-scomp5p  redefines       var-scomp5 pointer.
        02 var-scompu5  pic s9v9(10) computational-5 .
        02 var-scompu5p redefines       var-scompu5 pointer.
        procedure               division.
            move  0.0001193046 to var-binary var-comp var-compu
                                     var-comp4 var-compu4 var-comp5
                                     var-compu5
            display " " var-binary "  " var-comp "  " var-compu "  " 
                                    var-comp4 "  " var-compu4 "  " 
                                    var-comp5 "  " var-compu5
            move  0.0001193046 to var-sbinary var-scomp var-scompu
                                    var-scomp4 var-scompu4 var-scomp5
                                    var-scompu5
            display var-sbinary " " var-scomp " " var-scompu " " 
                                    var-scomp4 " " var-scompu4 " "
                                    var-scomp5 " " var-scompu5
            move -0.0001193046 to var-sbinary var-scomp var-scompu 
                                    var-scomp4 var-scompu4 var-scomp5 
                                    var-scompu5
            display var-sbinary " " var-scomp " " var-scompu " " 
                                    var-scomp4 " " var-scompu4 " " 
                                    var-scomp5 " " var-scompu5
            display var-binaryp
            display var-compp
            display var-compup
            display var-comp4p
            display var-compu4p
            display var-comp5p
            display var-compu5p

            display var-sbinaryp
            display var-scompp
            display var-scompup
            display var-scomp4p
            display var-scompu4p
            display var-scomp5p
            display var-scompu5p

            goback.
        end program             prog2.

