-- { dg-do compile }
-- { dg-options "-gnatws" }

with Ada.Containers.Indefinite_Ordered_Maps;
with Ada.Strings.Unbounded;

procedure Implicit_Deref1 is
   use Ada.Strings.Unbounded;

   package Reproduce_Pkg is
      type Person_Type is tagged private;

      type Person_Ref (Data : not null access Person_Type) is
      limited null record
      with Implicit_Dereference => Data;

      type Person_Handler_Ptr is access procedure (Person : Person_Type);

      function New_Person (Name : Unbounded_String) return Person_Ref;

      function New_Person
        (Person : aliased in out Person_Type; Name : Unbounded_String)
         return Person_Ref;

   private

      type Person_Type is tagged record
         Name : Unbounded_String;
      end record;
   end Reproduce_Pkg;

   package body Reproduce_Pkg is
      function New_Person (Name : Unbounded_String) return Person_Ref is
      begin
         return (Data => new Person_Type'(Name => Name));
      end New_Person;

      function New_Person
        (Person : aliased in out Person_Type; Name : Unbounded_String)
         return Person_Ref is
      begin
         return (Data => new Person_Type'(Name => Name));
      end New_Person;
   end Reproduce_Pkg;

   App : Reproduce_Pkg.Person_Type :=
     Reproduce_Pkg.New_Person (Name => To_Unbounded_String ("hello"));
begin
   null;
end;
