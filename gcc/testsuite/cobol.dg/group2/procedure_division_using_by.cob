       *> { dg-do run }
       id division.
      *> IBM is PERMISSIVE and allows PROGRAM-ID without a terminating
      *> period. ISO 2014 does not. IBM's behaviors are part of a
      *> DIALECT
       program-id prog.
       data division.
       working-storage section.
       77 var-1 pic x(10).
       77 var-2 pic x(10).
       procedure division.
           call "subroutine1" using by reference var-1
               by value var-2.
       id division.
       program-id. subroutine1.
       data division.
       linkage section.
       77 avar-1 pic x(10).
       77 avar-2 pic x(10).
       procedure division using by reference avar-1 by value avar-2.
           display avar-1.
           move "---" to avar-1.
           display avar-1.
           end program subroutine1.
           end program prog.

