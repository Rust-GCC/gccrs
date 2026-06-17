package Opt108_Pkg is

   procedure Set;

   procedure Put;

private

   type Test_Type is record
      X : Integer;
   end record;

   type Index_Type is range 1 .. 1;

   Cur_Index : Index_Type := 1;

   Flag : Boolean := False;

   Val : Test_Type;

end Opt108_Pkg;
