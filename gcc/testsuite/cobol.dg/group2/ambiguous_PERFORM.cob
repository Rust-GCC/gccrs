       *> { dg-do run }
       *> { dg-output-file "group2/ambiguous_PERFORM.out" }

       IDENTIFICATION DIVISION.
       PROGRAM-ID. prog.
       DATA DIVISION.
       WORKING-STORAGE SECTION. 
       PROCEDURE DIVISION.
       first-section section.
       paragraph-1.
          perform paragraph-2
          GO TO get-out.
       paragraph-2.
          display "paragraph 2 in first-section.".
          exit paragraph.
       get-out.
       GOBACK.
       
       second-section section.
       paragraph-2. 
          display "paragraph 2 in second-section.".

