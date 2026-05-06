MODULE setcons3 ;

TYPE
   Subrange = [10..40] ;
   Set = SET OF Subrange ;
   Rec = RECORD
            field: Subrange ;
         END ;

VAR
   u: Subrange ;
   s: Set ;
   r: Rec ;


PROCEDURE Init ;
BEGIN
   s := Set {u}
END Init ;


BEGIN
   Init
END setcons3.
