-- { dg-do compile }
-- { dg-options "-gnat2022" }

with Ada.Text_IO; use Ada.Text_IO;

procedure Reduce6 is

   type Rec1 is record
      A : Integer;
   end record;

   type Rec2 is record
      A : Integer;
   end record;

   function Init return Rec1 is (A => 0);
   function Init return Rec2 is (A => 1000); -- unused

   function Foo (X : Integer) return Integer is (X);
   function Foo (X : Integer) return Float is (0.0); -- unused

   function Reducer (X : Rec1; Y : Integer) return Rec1 is (A => X.A + Y);
   function Reducer (X : Rec2; Y : Float)   return Rec2 is (A => 2); -- unused
   function Reducer (X : Rec2; Y : Integer) return Rec2 is (A => 3); -- unused

   function Higher (X : Rec1; Y : Rec1)  return Rec1 is
     (if X.A >= Y.A then X else Y);
   function Higher (X : Rec1; Y : Float) return Rec1 is (X); -- unused

   R : Rec1 :=
     [for J in 1 .. 10 =>
       [for I in 1 .. J => Foo (I)]'Reduce (Reducer, Init)]'
         Reduce (Higher, Init);
begin
   Put_Line (R.A'Image); -- 55
end;
