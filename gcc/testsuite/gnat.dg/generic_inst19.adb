-- { dg-do compile }

with Ada.Containers.Vectors;

procedure Generic_Inst19 is

   generic
      type T is private;
      type Vector_Index is range <>;
      --  Index and T_Array need to be placed here or T actual unconstrained
      with package T_Vectors is new Ada.Containers.Vectors (Vector_Index, T);
      type Index is range <>;
      type T_Array is array (Index range <>) of T;
   function To_Array_Generic (Vec : T_Vectors.Vector) return T_Array;

   function To_Array_Generic (Vec : T_Vectors.Vector) return T_Array is
      Result : T_Array (Index'First .. Index'First + Index (Vec.Length) - 1);
      I      : Index := Index'First;
   begin
      for X of Vec loop
         Result (I) := X;
         I := I + 1;
      end loop;
      return Result;
   end To_Array_Generic;

   type Probability is new Float range 0.0 .. 1.0;

   package Probability_Vectors is new Ada.Containers.Vectors
      (Index_Type => Positive, Element_Type => Probability);
   type Probability_Array is array (Positive range <>) of Probability;

   function To_Array is new To_Array_Generic (
      T            => Probability,
      Vector_Index => Positive,
      T_Vectors    => Probability_Vectors,
      Index  => Positive,
      T_Array      => Probability_Array);

begin
   null;
end;
