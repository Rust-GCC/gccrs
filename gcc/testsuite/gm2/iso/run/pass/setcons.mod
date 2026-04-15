MODULE setcons ;

TYPE
   Subrange = [10..40] ;
   Set = SET OF Subrange ;

VAR
   u: Subrange ;
   s: Set ;


PROCEDURE Init ;
BEGIN
   u := MIN (u) ;
   s := Set {u}
END Init ;


BEGIN
   Init
END setcons.
