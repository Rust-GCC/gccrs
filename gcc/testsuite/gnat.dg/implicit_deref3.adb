-- { dg-do compile }

procedure Implicit_Deref3 is

   package P is
      type Root is tagged null record;

      type Access_Root_Class is access Root'Class;

      type Root_Ref is tagged record
         Block : Access_Root_Class;
      end record;

      function Create (Value : Root'Class) return Root_Ref
      is (Block => new Root'Class'(Value));
   end P;

   use P;

   generic
      type T (<>) is abstract new Root with private;
      type Parent_Ref is new Root_Ref with private;
   package Pointers is
      type Ref is new Parent_Ref with null record;

      not overriding
      function Create (Value : T) return Ref
      is (Parent_Ref'(Create (Value)) with null record);

      type Reference_Type (Data : access T) is limited null record
      with Implicit_Dereference => Data;

      function Get (This : Ref) return Reference_Type
      is (Data => T (This.Block.all)'Access);
   end Pointers;

   type Derived_A is abstract new Root with null record;

   type Derived_B is new Derived_A with record
      I : Integer;
   end record;

   package A_Pointers is new Pointers (Derived_A, Root_Ref);
   package B_Pointers is new Pointers (Derived_B, A_Pointers.Ref);

   X : B_Pointers.Ref;
   Y : Integer := X.Get.Data.I;
   Z : Integer := B_Pointers.Get (X).Data.I;

begin
   null;
end;
