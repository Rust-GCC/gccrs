-- { dg-do compile }

with Ada.Text_IO;
with System.Address_To_Access_Conversions;

procedure Implicit_Deref2 is

   package Test is

      type Any is
         tagged limited
            record
               Address : System.Address;
            end record;

      type Any_Ptr is access Any;

      generic
         type T (<>) is limited private;
         type T_Ptr is access T;
      function Make (P : T_Ptr) return Any_Ptr;

      generic
         type T (<>) is limited private;
         type T_Ref (Data : not null access constant T) is limited private;
         with function Make_T_Ref (Data : not null access constant T) return T_Ref;
      function Get (P : Any_Ptr) return T_Ref;

   end Test;

   package body Test is

      function Make (P : T_Ptr) return Any_Ptr is
         package Convert is new System.Address_To_Access_Conversions (T);
      begin
         return new Any'(Address => Convert.To_Address (Convert.Object_Pointer (P)));
      end Make;

      function Get (P : Any_Ptr) return T_Ref is
         package Convert is new System.Address_To_Access_Conversions (T);
      begin
         return Make_T_Ref (Convert.To_Pointer (P.Address));
      end Get;

   end Test;

   type Integer_Ptr is access Integer;
   type Integer_Ref (Data : not null access constant Integer) is limited null record
      with Implicit_Dereference => Data;
   function Make_Integer_Ref (Data : not null access constant Integer) return Integer_Ref is
     (Data => Data);

   function Make is new Test.Make (Integer, Integer_Ptr);
   function Get is new Test.Get (Integer, Integer_Ref, Make_Integer_Ref);

   type Float_Ptr is access Float;
   type Float_Ref (Data : not null access constant Float) is limited null record
      with Implicit_Dereference => Data;
   function Make_Float_Ref (Data : not null access constant Float) return Float_Ref is
     (Data => Data);

   function Make is new Test.Make (Float, Float_Ptr);
   function Get is new Test.Get (Float, Float_Ref, Make_Float_Ref);

   A1 : Test.Any_Ptr := Make (new Integer'(42));
   A2 : Integer := Get (A1);

   B1 : Test.Any_Ptr := Make (new Float'(43.0));
   B2 : Float := Get (B1);

begin
   Ada.Text_IO.Put_Line (A2'Image);
   Ada.Text_IO.Put_Line (B2'Image);
end;
