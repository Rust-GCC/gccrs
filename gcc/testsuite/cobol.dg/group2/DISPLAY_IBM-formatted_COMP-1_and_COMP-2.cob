       *> { dg-do run }
       *> { dg-options "-dialect ibm" }
       *> { dg-output-file "group2/DISPLAY_IBM-formatted_COMP-1_and_COMP-2.out" }
      * gcobol bug reproducer: DISPLAY of a COMP-1 / COMP-2 (internal floating-point)
      * item does not use the IBM external floating-point form.
      *
      * Per IBM Enterprise COBOL, DISPLAY of an internal floating-point item converts it
      * to external floating-point: a COMP-1 displays "as if it had an external
      * floating-point PICTURE clause of -.9(8)E-99", and a COMP-2 as -.9(17)E-99
      * (Language Reference SC27-8713-04, "DISPLAY statement", p.340).  The value is shown as
      *   <sign>.<mantissa>E<expsign><2-digit exponent>
      * with the mantissa normalized to a leading non-zero digit, and the sign / exponent
      * sign blank for non-negative.
      *
      * Observed (gcobol 15.2.0)        Expected (IBM LR external floating-point)
      *   1.5                             " .15000000E 01"
      *   0.1000000015                    " .10000000E 00"
      *   -2.75                           "-.27500000E 01"
      *   1500                            " .15000000E 04"
      *   2.71828000000000003             " .27182800000000000E 01"
      *
      * gcobol prints the plain decimal value (with binary-float artifacts) instead of the
      * documented external floating-point layout.  This reproduces with the default
      * dialect and also with "-dialect ibm" specified explicitly:
      *   gcobol -dialect ibm -o fltdisp float-display-not-external.cob && ./fltdisp
      * produces the same (non-conforming) output.
       IDENTIFICATION DIVISION.
       PROGRAM-ID. FLTDISP.
       DATA DIVISION.
       WORKING-STORAGE SECTION.
       01  F1 COMP-1 VALUE 1.5.
       01  F2 COMP-1 VALUE 0.1.
       01  F3 COMP-1 VALUE -2.75.
       01  F4 COMP-1 VALUE 1500.
       01  D1 COMP-2 VALUE 2.71828.
       01  D2 COMP-2 VALUE -.0000314159.
       PROCEDURE DIVISION.
           DISPLAY F1
           DISPLAY F2
           DISPLAY F3
           DISPLAY F4
           DISPLAY D1
           DISPLAY D2
           STOP RUN.

