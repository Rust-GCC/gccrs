MODULE setcons5 ;

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
   WITH r DO
      s := Set {field}
   END
END Init ;


BEGIN
   Init ;
END setcons5.
