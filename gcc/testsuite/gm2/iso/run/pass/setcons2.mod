MODULE setcons2 ;

FROM libc IMPORT exit, printf ;


TYPE
   Subrange = [10..40] ;
   Set = SET OF Subrange ;

VAR
   u: Subrange ;
   s: Set ;


PROCEDURE Assert (condition: BOOLEAN; testno: CARDINAL) ;
BEGIN
   IF NOT condition
   THEN
      printf ("assert %d failed\n", testno) ;
      exit (testno)
   END
END Assert ;



PROCEDURE Init ;
BEGIN
   u := MIN (u) ;
   s := Set {u} ;
   Assert (s = Set {10}, 1)
END Init ;


BEGIN
   Init ;
   printf ("all tests passed\n")
END setcons2.
