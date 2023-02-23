------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                                U N A M E                                 --
--                                                                          --
--                                 B o d y                                  --
--                                                                          --
--          Copyright (C) 1992-2023, Free Software Foundation, Inc.         --
--                                                                          --
-- GNAT is free software;  you can  redistribute it  and/or modify it under --
-- terms of the  GNU General Public License as published  by the Free Soft- --
-- ware  Foundation;  either version 3,  or (at your option) any later ver- --
-- sion.  GNAT is distributed in the hope that it will be useful, but WITH- --
-- OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY --
-- or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License --
-- for  more details.  You should have  received  a copy of the GNU General --
-- Public License  distributed with GNAT; see file COPYING3.  If not, go to --
-- http://www.gnu.org/licenses for a complete copy of the license.          --
--                                                                          --
-- GNAT was originally developed  by the GNAT team at  New York University. --
-- Extensive contributions were provided by Ada Core Technologies Inc.      --
--                                                                          --
------------------------------------------------------------------------------

with Atree;          use Atree;
with Casing;         use Casing;
with Einfo;          use Einfo;
with Einfo.Utils;    use Einfo.Utils;
with Hostparm;
with Lib;            use Lib;
with Nlists;         use Nlists;
with Output;         use Output;
with Sinfo;          use Sinfo;
with Sinfo.Nodes;    use Sinfo.Nodes;
with Sinfo.Utils;    use Sinfo.Utils;
with Sinput;         use Sinput;

package body Uname is

   function Has_Prefix (X, Prefix : String) return Boolean;
   --  True if Prefix is at the beginning of X. For example,
   --  Has_Prefix("a-filename.ads", Prefix => "a-") is True.

   -------------------
   -- Get_Body_Name --
   -------------------

   function Get_Body_Name (N : Unit_Name_Type) return Unit_Name_Type is
      Buffer : Bounded_String;
   begin
      Append (Buffer, N);

      pragma Assert
        (Buffer.Length > 2
         and then Buffer.Chars (Buffer.Length - 1) = '%'
         and then Buffer.Chars (Buffer.Length) = 's');

      Buffer.Chars (Buffer.Length) := 'b';

      return Name_Find (Buffer);
   end Get_Body_Name;

   -----------------------------------
   -- Get_External_Unit_Name_String --
   -----------------------------------

   procedure Get_External_Unit_Name_String (N : Unit_Name_Type) is
      Pcount : Natural;
      Newlen : Natural;

   begin
      --  Get unit name and eliminate trailing %s or %b

      Get_Name_String (N);
      Name_Len := Name_Len - 2;

      --  Find number of components

      Pcount := 0;
      for J in 1 .. Name_Len loop
         if Name_Buffer (J) = '.' then
            Pcount := Pcount + 1;
         end if;
      end loop;

      --  If simple name, nothing to do

      if Pcount = 0 then
         return;
      end if;

      --  If name has multiple components, replace dots by double underscore

      Newlen := Name_Len + Pcount;

      for J in reverse 1 .. Name_Len loop
         if Name_Buffer (J) = '.' then
            Name_Buffer (Newlen) := '_';
            Name_Buffer (Newlen - 1) := '_';
            Newlen := Newlen - 2;

         else
            Name_Buffer (Newlen) := Name_Buffer (J);
            Newlen := Newlen - 1;
         end if;
      end loop;

      Name_Len := Name_Len + Pcount;
   end Get_External_Unit_Name_String;

   --------------------------
   -- Get_Parent_Body_Name --
   --------------------------

   function Get_Parent_Body_Name (N : Unit_Name_Type) return Unit_Name_Type is
      Buffer : Bounded_String;
   begin
      Append (Buffer, N);

      while Buffer.Chars (Buffer.Length) /= '.' loop
         pragma Assert (Buffer.Length > 1); -- not a child or subunit name
         Buffer.Length := Buffer.Length - 1;
      end loop;

      Buffer.Chars (Buffer.Length) := '%';
      Append (Buffer, 'b');

      return Name_Find (Buffer);
   end Get_Parent_Body_Name;

   --------------------------
   -- Get_Parent_Spec_Name --
   --------------------------

   function Get_Parent_Spec_Name (N : Unit_Name_Type) return Unit_Name_Type is
      Buffer : Bounded_String;
   begin
      Append (Buffer, N);

      while Buffer.Chars (Buffer.Length) /= '.' loop
         if Buffer.Length = 1 then
            return No_Unit_Name;
         else
            Buffer.Length := Buffer.Length - 1;
         end if;
      end loop;

      Buffer.Chars (Buffer.Length) := '%';
      Append (Buffer, 's');

      return Name_Find (Buffer);
   end Get_Parent_Spec_Name;

   -------------------
   -- Get_Spec_Name --
   -------------------

   function Get_Spec_Name (N : Unit_Name_Type) return Unit_Name_Type is
      Buffer : Bounded_String;
   begin
      Append (Buffer, N);

      pragma Assert
        (Buffer.Length > 2
         and then Buffer.Chars (Buffer.Length - 1) = '%'
         and then Buffer.Chars (Buffer.Length) = 'b');

      Buffer.Chars (Buffer.Length) := 's';

      return Name_Find (Buffer);
   end Get_Spec_Name;

   -------------------
   -- Get_Unit_Name --
   -------------------

   function Get_Unit_Name (N : Node_Id) return Unit_Name_Type is

      Unit_Name_Buffer : Bounded_String;
      --  Buffer used to build name of unit

      Node : Node_Id;
      --  Program unit node

      procedure Add_Char (C : Character);
      --  Add a single character to stored unit name

      procedure Add_Name (Name : Name_Id);
      --  Add the characters of a names table entry to stored unit name

      procedure Add_Node_Name (Node : Node_Id);
      --  Recursive procedure adds characters associated with Node

      function Get_Parent (Node : Node_Id) return Node_Id;
      --  Get parent compilation unit of a stub

      --------------
      -- Add_Char --
      --------------

      procedure Add_Char (C : Character) is
      begin
         Append (Unit_Name_Buffer, C);
      end Add_Char;

      --------------
      -- Add_Name --
      --------------

      procedure Add_Name (Name : Name_Id) is
      begin
         Append (Unit_Name_Buffer, Name);
      end Add_Name;

      -------------------
      -- Add_Node_Name --
      -------------------

      procedure Add_Node_Name (Node : Node_Id) is
      begin
         --  Just ignore an error node (someone else will give a message)

         if Node = Error then
            return;

         --  Otherwise see what kind of node we have

         else
            case Nkind (Node) is
               when N_Defining_Identifier
                  | N_Defining_Operator_Symbol
                  | N_Identifier
               =>
                  --  Note: it is of course an error to have a defining
                  --  operator symbol at this point, but this is not where
                  --  the error is signalled, so we handle it nicely here.

                  Add_Name (Chars (Node));

               when N_Defining_Program_Unit_Name =>
                  Add_Node_Name (Name (Node));
                  Add_Char ('.');
                  Add_Node_Name (Defining_Identifier (Node));

               when N_Expanded_Name
                  | N_Selected_Component
               =>
                  Add_Node_Name (Prefix (Node));
                  Add_Char ('.');
                  Add_Node_Name (Selector_Name (Node));

               when N_Package_Specification
                  | N_Subprogram_Specification
               =>
                  Add_Node_Name (Defining_Unit_Name (Node));

               when N_Generic_Declaration
                  | N_Package_Declaration
                  | N_Subprogram_Body
                  | N_Subprogram_Declaration
               =>
                  Add_Node_Name (Specification (Node));

               when N_Generic_Instantiation =>
                  Add_Node_Name (Defining_Unit_Name (Node));

               when N_Package_Body =>
                  Add_Node_Name (Defining_Unit_Name (Node));

               when N_Protected_Body
                  | N_Task_Body
               =>
                  Add_Node_Name (Defining_Identifier (Node));

               when N_Package_Renaming_Declaration =>
                  Add_Node_Name (Defining_Unit_Name (Node));

               when N_Subprogram_Renaming_Declaration =>
                  Add_Node_Name (Specification (Node));

               when N_Generic_Renaming_Declaration =>
                  Add_Node_Name (Defining_Unit_Name (Node));

               when N_Subprogram_Body_Stub =>
                  Add_Node_Name (Get_Parent (Node));
                  Add_Char ('.');
                  Add_Node_Name (Specification (Node));

               when N_Compilation_Unit =>
                  Add_Node_Name (Unit (Node));

               when N_Package_Body_Stub
                  | N_Protected_Body_Stub
                  | N_Task_Body_Stub
               =>
                  Add_Node_Name (Get_Parent (Node));
                  Add_Char ('.');
                  Add_Node_Name (Defining_Identifier (Node));

               when N_Subunit =>
                  Add_Node_Name (Name (Node));
                  Add_Char ('.');
                  Add_Node_Name (Proper_Body (Node));

               when N_With_Clause =>
                  Add_Node_Name (Name (Node));

               when N_Pragma =>
                  Add_Node_Name (Expression (First
                    (Pragma_Argument_Associations (Node))));

               --  Tasks and protected stuff appear only in an error context,
               --  but the error has been posted elsewhere, so we deal nicely
               --  with these error situations here, and produce a reasonable
               --  unit name using the defining identifier.

               when N_Protected_Type_Declaration
                  | N_Single_Protected_Declaration
                  | N_Single_Task_Declaration
                  | N_Task_Type_Declaration
               =>
                  Add_Node_Name (Defining_Identifier (Node));

               when others =>
                  raise Program_Error;
            end case;
         end if;
      end Add_Node_Name;

      ----------------
      -- Get_Parent --
      ----------------

      function Get_Parent (Node : Node_Id) return Node_Id is
         N : Node_Id := Node;

      begin
         while Nkind (N) /= N_Compilation_Unit loop
            N := Parent (N);
         end loop;

         return N;
      end Get_Parent;

   --  Start of processing for Get_Unit_Name

   begin
      Node := N;

      --  If we have Defining_Identifier, find the associated unit node

      if Nkind (Node) = N_Defining_Identifier then
         Node := Declaration_Node (Node);

      --  If an expanded name, it is an already analyzed child unit, find
      --  unit node.

      elsif Nkind (Node) = N_Expanded_Name then
         Node := Declaration_Node (Entity (Node));
      end if;

      if Nkind (Node) in N_Package_Specification
                       | N_Subprogram_Specification
      then
         Node := Parent (Node);
      end if;

      --  Node points to the unit, so get its name and add proper suffix

      Add_Node_Name (Node);
      Add_Char ('%');

      case Nkind (Node) is
         when N_Generic_Declaration
            | N_Generic_Instantiation
            | N_Generic_Renaming_Declaration
            | N_Package_Declaration
            | N_Package_Renaming_Declaration
            | N_Pragma
            | N_Protected_Type_Declaration
            | N_Single_Protected_Declaration
            | N_Single_Task_Declaration
            | N_Subprogram_Declaration
            | N_Subprogram_Renaming_Declaration
            | N_Task_Type_Declaration
            | N_With_Clause
         =>
            Add_Char ('s');

         when N_Body_Stub
            | N_Identifier
            | N_Package_Body
            | N_Protected_Body
            | N_Selected_Component
            | N_Subprogram_Body
            | N_Subunit
            | N_Task_Body
         =>
            Add_Char ('b');

         when others =>
            raise Program_Error;
      end case;

      return Name_Find (Unit_Name_Buffer);
   end Get_Unit_Name;

   --------------------------
   -- Get_Unit_Name_String --
   --------------------------

   procedure Get_Unit_Name_String
     (Buf    : in out Bounded_String;
      N      : Unit_Name_Type;
      Suffix : Boolean := True)
   is
   begin
      Buf.Length := 0;
      Append_Decoded (Buf, N);

      --  Buf always ends with "%s" or "%b", which we either remove, or replace
      --  with " (spec)" or " (body)". Set_Casing of Buf after checking for
      --  (lower case) 's'/'b', and before appending (lower case) "spec" or
      --  "body".

      pragma Assert (Buf.Length >= 3);
      pragma Assert (Buf.Chars (1) /= '"');
      pragma Assert (Buf.Chars (Buf.Length) in 's' | 'b');

      declare
         S : constant String :=
           (if Buf.Chars (Buf.Length) = 's' then " (spec)" else " (body)");
      begin
         Buf.Length := Buf.Length - 1; -- remove 's' or 'b'
         pragma Assert (Buf.Chars (Buf.Length) = '%');
         Buf.Length := Buf.Length - 1; -- remove '%'
         Set_Casing (Buf, Identifier_Casing (Source_Index (Main_Unit)));

         if Suffix then
            Append (Buf, S);
         end if;
      end;

      for J in 1 .. Buf.Length loop
         if Buf.Chars (J) = '-' then
            Buf.Chars (J) := '.';
         end if;
      end loop;
   end Get_Unit_Name_String;

   ----------------
   -- Has_Prefix --
   ----------------

   function Has_Prefix (X, Prefix : String) return Boolean is
   begin
      if X'Length >= Prefix'Length then
         declare
            Slice : String renames
                      X (X'First .. X'First + Prefix'Length - 1);
         begin
            return Slice = Prefix;
         end;
      end if;
      return False;
   end Has_Prefix;

   ------------------
   -- Is_Body_Name --
   ------------------

   function Is_Body_Name (N : Unit_Name_Type) return Boolean is
      Buffer : Bounded_String;
   begin
      Append (Buffer, N);
      return Buffer.Length > 2
        and then Buffer.Chars (Buffer.Length - 1) = '%'
        and then Buffer.Chars (Buffer.Length) = 'b';
   end Is_Body_Name;

   -------------------
   -- Is_Child_Name --
   -------------------

   function Is_Child_Name (N : Unit_Name_Type) return Boolean is
      Buffer : Bounded_String;

   begin
      Append (Buffer, N);

      while Buffer.Chars (Buffer.Length) /= '.' loop
         if Buffer.Length = 1 then
            return False; -- not a child or subunit name
         else
            Buffer.Length := Buffer.Length - 1;
         end if;
      end loop;

      return True;
   end Is_Child_Name;

   ---------------------------
   -- Is_Internal_Unit_Name --
   ---------------------------

   function Is_Internal_Unit_Name
     (Name               : String;
      Renamings_Included : Boolean := True) return Boolean
   is
      Gnat : constant String := "gnat";

   begin
      if Name = Gnat then
         return True;
      end if;

      if Has_Prefix (Name, Prefix => Gnat & ".") then
         return True;
      end if;

      return Is_Predefined_Unit_Name (Name, Renamings_Included);
   end Is_Internal_Unit_Name;

   -----------------------------
   -- Is_Predefined_Unit_Name --
   -----------------------------

   function Is_Predefined_Unit_Name
     (Name               : String;
      Renamings_Included : Boolean := True) return Boolean
   is
      Ada        : constant String := "ada";
      Interfaces : constant String := "interfaces";
      System     : constant String := "system";

   begin
      if Name = Ada
        or else Name = Interfaces
        or else Name = System
      then
         return True;
      end if;

      if Has_Prefix (Name, Prefix => Ada & ".")
        or else Has_Prefix (Name, Prefix => Interfaces & ".")
        or else Has_Prefix (Name, Prefix => System & ".")
      then
         return True;
      end if;

      if not Renamings_Included then
         return False;
      end if;

      --  The following are the predefined renamings

      return
        Name = "calendar"
          or else Name = "machine_code"
          or else Name = "unchecked_conversion"
          or else Name = "unchecked_deallocation"
          or else Name = "direct_io"
          or else Name = "io_exceptions"
          or else Name = "sequential_io"
          or else Name = "text_io";
   end Is_Predefined_Unit_Name;

   ------------------
   -- Is_Spec_Name --
   ------------------

   function Is_Spec_Name (N : Unit_Name_Type) return Boolean is
      Buffer : Bounded_String;
   begin
      Append (Buffer, N);
      return Buffer.Length > 2
        and then Buffer.Chars (Buffer.Length - 1) = '%'
        and then Buffer.Chars (Buffer.Length) = 's';
   end Is_Spec_Name;

   -----------------------
   -- Name_To_Unit_Name --
   -----------------------

   function Name_To_Unit_Name (N : Name_Id) return Unit_Name_Type is
      Buffer : Bounded_String;
   begin
      Append (Buffer, N);
      Append (Buffer, "%s");
      return Name_Find (Buffer);
   end Name_To_Unit_Name;

   ---------------
   -- New_Child --
   ---------------

   function New_Child
     (Old  : Unit_Name_Type;
      Newp : Unit_Name_Type) return Unit_Name_Type
   is
      P : Natural;

   begin
      Get_Name_String (Old);

      declare
         Child : constant String := Name_Buffer (1 .. Name_Len);

      begin
         Get_Name_String (Newp);
         Name_Len := Name_Len - 2;

         P := Child'Last;
         while Child (P) /= '.' loop
            P := P - 1;
         end loop;

         while P <= Child'Last loop
            Name_Len := Name_Len + 1;
            Name_Buffer (Name_Len) := Child (P);
            P := P + 1;
         end loop;

         return Name_Find;
      end;
   end New_Child;

   --------------
   -- Uname_Ge --
   --------------

   function Uname_Ge (Left, Right : Unit_Name_Type) return Boolean is
   begin
      return Left = Right or else Uname_Gt (Left, Right);
   end Uname_Ge;

   --------------
   -- Uname_Gt --
   --------------

   function Uname_Gt (Left, Right : Unit_Name_Type) return Boolean is
   begin
      return Left /= Right and then not Uname_Lt (Left, Right);
   end Uname_Gt;

   --------------
   -- Uname_Le --
   --------------

   function Uname_Le (Left, Right : Unit_Name_Type) return Boolean is
   begin
      return Left = Right or else Uname_Lt (Left, Right);
   end Uname_Le;

   --------------
   -- Uname_Lt --
   --------------

   function Uname_Lt (Left, Right : Unit_Name_Type) return Boolean is
      Left_Name    : String (1 .. Hostparm.Max_Name_Length);
      Left_Length  : Natural;
      Right_Name   : String renames Name_Buffer;
      Right_Length : Natural renames Name_Len;
      J            : Natural;

   begin
      pragma Warnings (Off, Right_Length);
      --  Suppress warnings on Right_Length, used in pragma Assert

      if Left = Right then
         return False;
      end if;

      Get_Name_String (Left);
      Left_Name  (1 .. Name_Len + 1) := Name_Buffer (1 .. Name_Len + 1);
      Left_Length := Name_Len;
      Get_Name_String (Right);
      J := 1;

      loop
         exit when Left_Name (J) = '%';

         if Right_Name (J) = '%' then
            return False; -- left name is longer
         end if;

         pragma Assert (J <= Left_Length and then J <= Right_Length);

         if Left_Name (J) /= Right_Name (J) then
            return Left_Name (J) < Right_Name (J); -- parent names different
         end if;

         J := J + 1;
      end loop;

      --  Come here pointing to % in left name

      if Right_Name (J) /= '%' then
         return True; -- right name is longer
      end if;

      --  Here the parent names are the same and specs sort low. If neither is
      --  a spec, then we are comparing the same name and we want a result of
      --  False in any case.

      return Left_Name (J + 1) = 's';
   end Uname_Lt;

   ---------------------
   -- Write_Unit_Name --
   ---------------------

   procedure Write_Unit_Name (N : Unit_Name_Type) is
      Buf : Bounded_String;
   begin
      Get_Unit_Name_String (Buf, N);
      Write_Str (Buf.Chars (1 .. Buf.Length));
   end Write_Unit_Name;

   -------------------------------
   -- Write_Unit_Name_For_Debug --
   -------------------------------

   procedure Write_Unit_Name_For_Debug (N : Unit_Name_Type) is
   begin
      if Is_Valid_Name (N) then
         Write_Unit_Name (N);
      else
         Write_Name_For_Debug (N);
      end if;
   end Write_Unit_Name_For_Debug;

end Uname;
