------------------------------------------------------------------------------
--                                                                          --
--                         GNAT LIBRARY COMPONENTS                          --
--                                                                          --
--            A D A . C O N T A I N E R S . H A S H E D _ M A P S           --
--                                                                          --
--                                 B o d y                                  --
--                                                                          --
--          Copyright (C) 2004-2019, Free Software Foundation, Inc.         --
--                                                                          --
-- GNAT is free software;  you can  redistribute it  and/or modify it under --
-- terms of the  GNU General Public License as published  by the Free Soft- --
-- ware  Foundation;  either version 3,  or (at your option) any later ver- --
-- sion.  GNAT is distributed in the hope that it will be useful, but WITH- --
-- OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY --
-- or FITNESS FOR A PARTICULAR PURPOSE.                                     --
--                                                                          --
-- As a special exception under Section 7 of GPL version 3, you are granted --
-- additional permissions described in the GCC Runtime Library Exception,   --
-- version 3.1, as published by the Free Software Foundation.               --
--                                                                          --
-- You should have received a copy of the GNU General Public License and    --
-- a copy of the GCC Runtime Library Exception along with this program;     --
-- see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see    --
-- <http://www.gnu.org/licenses/>.                                          --
--                                                                          --
-- This unit was originally developed by Matthew J Heaney.                  --
------------------------------------------------------------------------------

with Ada.Unchecked_Deallocation;

with Ada.Containers.Hash_Tables.Generic_Operations;
pragma Elaborate_All (Ada.Containers.Hash_Tables.Generic_Operations);

with Ada.Containers.Hash_Tables.Generic_Keys;
pragma Elaborate_All (Ada.Containers.Hash_Tables.Generic_Keys);

with Ada.Containers.Helpers; use Ada.Containers.Helpers;

with System; use type System.Address;

package body Ada.Containers.Hashed_Maps is

   pragma Warnings (Off, "variable ""Busy*"" is not referenced");
   pragma Warnings (Off, "variable ""Lock*"" is not referenced");
   --  See comment in Ada.Containers.Helpers

   -----------------------
   -- Local Subprograms --
   -----------------------

   function Copy_Node
     (Source : Node_Access) return Node_Access;
   pragma Inline (Copy_Node);

   function Equivalent_Key_Node
     (Key  : Key_Type;
      Node : Node_Access) return Boolean;
   pragma Inline (Equivalent_Key_Node);

   procedure Free (X : in out Node_Access);

   function Find_Equal_Key
     (R_HT   : Hash_Table_Type;
      L_Node : Node_Access) return Boolean;

   function Hash_Node (Node : Node_Access) return Hash_Type;
   pragma Inline (Hash_Node);

   function Next (Node : Node_Access) return Node_Access;
   pragma Inline (Next);

   function Read_Node
     (Stream : not null access Root_Stream_Type'Class) return Node_Access;
   pragma Inline (Read_Node);

   procedure Set_Next (Node : Node_Access; Next : Node_Access);
   pragma Inline (Set_Next);

   function Vet (Position : Cursor) return Boolean;

   procedure Write_Node
     (Stream : not null access Root_Stream_Type'Class;
      Node   : Node_Access);
   pragma Inline (Write_Node);

   --------------------------
   -- Local Instantiations --
   --------------------------

   package HT_Ops is new Hash_Tables.Generic_Operations
     (HT_Types  => HT_Types,
      Hash_Node => Hash_Node,
      Next      => Next,
      Set_Next  => Set_Next,
      Copy_Node => Copy_Node,
      Free      => Free);

   package Key_Ops is new Hash_Tables.Generic_Keys
     (HT_Types        => HT_Types,
      Next            => Next,
      Set_Next        => Set_Next,
      Key_Type        => Key_Type,
      Hash            => Hash,
      Equivalent_Keys => Equivalent_Key_Node);

   function Is_Equal is new HT_Ops.Generic_Equal (Find_Equal_Key);

   procedure Read_Nodes  is new HT_Ops.Generic_Read (Read_Node);
   procedure Write_Nodes is new HT_Ops.Generic_Write (Write_Node);

   ---------
   -- "=" --
   ---------

   function "=" (Left, Right : Map) return Boolean is
   begin
      return Is_Equal (Left.HT, Right.HT);
   end "=";

   ------------
   -- Adjust --
   ------------

   procedure Adjust (Container : in out Map) is
   begin
      HT_Ops.Adjust (Container.HT);
   end Adjust;

   ------------
   -- Assign --
   ------------

   procedure Assign (Target : in out Map; Source : Map) is
      procedure Insert_Item (Node : Node_Access);
      pragma Inline (Insert_Item);

      procedure Insert_Items is new HT_Ops.Generic_Iteration (Insert_Item);

      -----------------
      -- Insert_Item --
      -----------------

      procedure Insert_Item (Node : Node_Access) is
      begin
         Target.Insert (Key => Node.Key, New_Item => Node.Element);
      end Insert_Item;

   --  Start of processing for Assign

   begin
      if Target'Address = Source'Address then
         return;
      end if;

      Target.Clear;

      if Target.Capacity < Source.Length then
         Target.Reserve_Capacity (Source.Length);
      end if;

      Insert_Items (Source.HT);
   end Assign;

   --------------
   -- Capacity --
   --------------

   function Capacity (Container : Map) return Count_Type is
   begin
      return HT_Ops.Capacity (Container.HT);
   end Capacity;

   -----------
   -- Clear --
   -----------

   procedure Clear (Container : in out Map) is
   begin
      HT_Ops.Clear (Container.HT);
   end Clear;

   ------------------------
   -- Constant_Reference --
   ------------------------

   function Constant_Reference
     (Container : aliased Map;
      Position  : Cursor) return Constant_Reference_Type
   is
   begin
      if Checks and then Position.Container = null then
         raise Constraint_Error with
           "Position cursor has no element";
      end if;

      if Checks and then Position.Container /= Container'Unrestricted_Access
      then
         raise Program_Error with
           "Position cursor designates wrong map";
      end if;

      pragma Assert
        (Vet (Position),
         "Position cursor in Constant_Reference is bad");

      declare
         HT : Hash_Table_Type renames Container'Unrestricted_Access.all.HT;
         TC : constant Tamper_Counts_Access :=
           HT.TC'Unrestricted_Access;
      begin
         return R : constant Constant_Reference_Type :=
           (Element => Position.Node.Element'Access,
            Control => (Controlled with TC))
         do
            Busy (TC.all);
         end return;
      end;
   end Constant_Reference;

   function Constant_Reference
     (Container : aliased Map;
      Key       : Key_Type) return Constant_Reference_Type
   is
      HT   : Hash_Table_Type renames Container'Unrestricted_Access.HT;
      Node : constant Node_Access := Key_Ops.Find (HT, Key);

   begin
      if Checks and then Node = null then
         raise Constraint_Error with "key not in map";
      end if;

      declare
         TC : constant Tamper_Counts_Access :=
           HT.TC'Unrestricted_Access;
      begin
         return R : constant Constant_Reference_Type :=
           (Element => Node.Element'Access,
            Control => (Controlled with TC))
         do
            Busy (TC.all);
         end return;
      end;
   end Constant_Reference;

   --------------
   -- Contains --
   --------------

   function Contains (Container : Map; Key : Key_Type) return Boolean is
   begin
      return Find (Container, Key) /= No_Element;
   end Contains;

   ----------
   -- Copy --
   ----------

   function Copy
     (Source   : Map;
      Capacity : Count_Type := 0) return Map
   is
      C : Count_Type;

   begin
      if Capacity < Source.Length then
         if Checks and then Capacity /= 0 then
            raise Capacity_Error
              with "Requested capacity is less than Source length";
         end if;

         C := Source.Length;
      else
         C := Capacity;
      end if;

      return Target : Map do
         Target.Reserve_Capacity (C);
         Target.Assign (Source);
      end return;
   end Copy;

   ---------------
   -- Copy_Node --
   ---------------

   function Copy_Node
     (Source : Node_Access) return Node_Access
   is
      Target : constant Node_Access :=
        new Node_Type'(Key     => Source.Key,
                       Element => Source.Element,
                       Next    => null);
   begin
      return Target;
   end Copy_Node;

   ------------
   -- Delete --
   ------------

   procedure Delete (Container : in out Map; Key : Key_Type) is
      X : Node_Access;

   begin
      Key_Ops.Delete_Key_Sans_Free (Container.HT, Key, X);

      if Checks and then X = null then
         raise Constraint_Error with "attempt to delete key not in map";
      end if;

      Free (X);
   end Delete;

   procedure Delete (Container : in out Map; Position : in out Cursor) is
   begin
      if Checks and then Position.Node = null then
         raise Constraint_Error with
           "Position cursor of Delete equals No_Element";
      end if;

      if Checks and then Position.Container /= Container'Unrestricted_Access
      then
         raise Program_Error with
           "Position cursor of Delete designates wrong map";
      end if;

      TC_Check (Container.HT.TC);

      pragma Assert (Vet (Position), "bad cursor in Delete");

      HT_Ops.Delete_Node_Sans_Free (Container.HT, Position.Node);

      Free (Position.Node);
      Position.Container := null;
   end Delete;

   -------------
   -- Element --
   -------------

   function Element (Container : Map; Key : Key_Type) return Element_Type is
      HT   : Hash_Table_Type renames Container'Unrestricted_Access.HT;
      Node : constant Node_Access := Key_Ops.Find (HT, Key);

   begin
      if Checks and then Node = null then
         raise Constraint_Error with
           "no element available because key not in map";
      end if;

      return Node.Element;
   end Element;

   function Element (Position : Cursor) return Element_Type is
   begin
      if Checks and then Position.Node = null then
         raise Constraint_Error with
           "Position cursor of function Element equals No_Element";
      end if;

      pragma Assert (Vet (Position), "bad cursor in function Element");

      return Position.Node.Element;
   end Element;

   -------------------------
   -- Equivalent_Key_Node --
   -------------------------

   function Equivalent_Key_Node
     (Key  : Key_Type;
      Node : Node_Access) return Boolean is
   begin
      return Equivalent_Keys (Key, Node.Key);
   end Equivalent_Key_Node;

   ---------------------
   -- Equivalent_Keys --
   ---------------------

   function Equivalent_Keys (Left, Right : Cursor)
     return Boolean is
   begin
      if Checks and then Left.Node = null then
         raise Constraint_Error with
           "Left cursor of Equivalent_Keys equals No_Element";
      end if;

      if Checks and then Right.Node = null then
         raise Constraint_Error with
           "Right cursor of Equivalent_Keys equals No_Element";
      end if;

      pragma Assert (Vet (Left), "Left cursor of Equivalent_Keys is bad");
      pragma Assert (Vet (Right), "Right cursor of Equivalent_Keys is bad");

      return Equivalent_Keys (Left.Node.Key, Right.Node.Key);
   end Equivalent_Keys;

   function Equivalent_Keys (Left : Cursor; Right : Key_Type) return Boolean is
   begin
      if Checks and then Left.Node = null then
         raise Constraint_Error with
           "Left cursor of Equivalent_Keys equals No_Element";
      end if;

      pragma Assert (Vet (Left), "Left cursor in Equivalent_Keys is bad");

      return Equivalent_Keys (Left.Node.Key, Right);
   end Equivalent_Keys;

   function Equivalent_Keys (Left : Key_Type; Right : Cursor) return Boolean is
   begin
      if Checks and then Right.Node = null then
         raise Constraint_Error with
           "Right cursor of Equivalent_Keys equals No_Element";
      end if;

      pragma Assert (Vet (Right), "Right cursor of Equivalent_Keys is bad");

      return Equivalent_Keys (Left, Right.Node.Key);
   end Equivalent_Keys;

   -------------
   -- Exclude --
   -------------

   procedure Exclude (Container : in out Map; Key : Key_Type) is
      X : Node_Access;
   begin
      Key_Ops.Delete_Key_Sans_Free (Container.HT, Key, X);
      Free (X);
   end Exclude;

   --------------
   -- Finalize --
   --------------

   procedure Finalize (Container : in out Map) is
   begin
      HT_Ops.Finalize (Container.HT);
   end Finalize;

   procedure Finalize (Object : in out Iterator) is
   begin
      if Object.Container /= null then
         Unbusy (Object.Container.HT.TC);
      end if;
   end Finalize;

   ----------
   -- Find --
   ----------

   function Find (Container : Map; Key : Key_Type) return Cursor is
      HT   : Hash_Table_Type renames Container'Unrestricted_Access.HT;
      Node : constant Node_Access := Key_Ops.Find (HT, Key);

   begin
      if Node = null then
         return No_Element;
      end if;

      return Cursor'(Container'Unrestricted_Access, Node, Hash_Type'Last);
   end Find;

   --------------------
   -- Find_Equal_Key --
   --------------------

   function Find_Equal_Key
     (R_HT   : Hash_Table_Type;
      L_Node : Node_Access) return Boolean
   is
      R_Index : constant Hash_Type := Key_Ops.Index (R_HT, L_Node.Key);
      R_Node  : Node_Access := R_HT.Buckets (R_Index);

   begin
      while R_Node /= null loop
         if Equivalent_Keys (L_Node.Key, R_Node.Key) then
            return L_Node.Element = R_Node.Element;
         end if;

         R_Node := R_Node.Next;
      end loop;

      return False;
   end Find_Equal_Key;

   -----------
   -- First --
   -----------

   function First (Container : Map) return Cursor is
      Pos  : Hash_Type;
      Node : constant Node_Access := HT_Ops.First (Container.HT, Pos);
   begin
      if Node = null then
         return No_Element;
      end if;

      return Cursor'(Container'Unrestricted_Access, Node, Pos);
   end First;

   function First (Object : Iterator) return Cursor is
   begin
      return Object.Container.First;
   end First;

   ----------
   -- Free --
   ----------

   procedure Free (X : in out Node_Access) is
      procedure Deallocate is
         new Ada.Unchecked_Deallocation (Node_Type, Node_Access);
   begin
      if X /= null then
         X.Next := X;     --  detect mischief (in Vet)
         Deallocate (X);
      end if;
   end Free;

   ------------------------
   -- Get_Element_Access --
   ------------------------

   function Get_Element_Access
     (Position : Cursor) return not null Element_Access is
   begin
      return Position.Node.Element'Access;
   end Get_Element_Access;

   -----------------
   -- Has_Element --
   -----------------

   function Has_Element (Position : Cursor) return Boolean is
   begin
      pragma Assert (Vet (Position), "bad cursor in Has_Element");
      return Position.Node /= null;
   end Has_Element;

   ---------------
   -- Hash_Node --
   ---------------

   function Hash_Node (Node : Node_Access) return Hash_Type is
   begin
      return Hash (Node.Key);
   end Hash_Node;

   -------------
   -- Include --
   -------------

   procedure Include
     (Container : in out Map;
      Key       : Key_Type;
      New_Item  : Element_Type)
   is
      Position : Cursor;
      Inserted : Boolean;

   begin
      Insert (Container, Key, New_Item, Position, Inserted);

      if not Inserted then
         TE_Check (Container.HT.TC);

         Position.Node.Key := Key;
         Position.Node.Element := New_Item;
      end if;
   end Include;

   ------------
   -- Insert --
   ------------

   procedure Insert
     (Container : in out Map;
      Key       : Key_Type;
      Position  : out Cursor;
      Inserted  : out Boolean)
   is
      function New_Node (Next : Node_Access) return Node_Access;
      pragma Inline (New_Node);

      procedure Local_Insert is
        new Key_Ops.Generic_Conditional_Insert (New_Node);

      --------------
      -- New_Node --
      --------------

      function New_Node (Next : Node_Access) return Node_Access is
      begin
         return new Node_Type'(Key     => Key,
                               Element => <>,
                               Next    => Next);
      end New_Node;

      HT : Hash_Table_Type renames Container.HT;

   --  Start of processing for Insert

   begin
      if HT_Ops.Capacity (HT) = 0 then
         HT_Ops.Reserve_Capacity (HT, 1);
      end if;

      Local_Insert (HT, Key, Position.Node, Inserted);

      if Inserted
        and then HT.Length > HT_Ops.Capacity (HT)
      then
         HT_Ops.Reserve_Capacity (HT, HT.Length);
      end if;

      Position.Container := Container'Unrestricted_Access;
   end Insert;

   procedure Insert
     (Container : in out Map;
      Key       : Key_Type;
      New_Item  : Element_Type;
      Position  : out Cursor;
      Inserted  : out Boolean)
   is
      function New_Node (Next : Node_Access) return Node_Access;
      pragma Inline (New_Node);

      procedure Local_Insert is
        new Key_Ops.Generic_Conditional_Insert (New_Node);

      --------------
      -- New_Node --
      --------------

      function New_Node (Next : Node_Access) return Node_Access is
      begin
         return new Node_Type'(Key, New_Item, Next);
      end New_Node;

      HT : Hash_Table_Type renames Container.HT;

   --  Start of processing for Insert

   begin
      if HT_Ops.Capacity (HT) = 0 then
         HT_Ops.Reserve_Capacity (HT, 1);
      end if;

      Local_Insert (HT, Key, Position.Node, Inserted);

      if Inserted
        and then HT.Length > HT_Ops.Capacity (HT)
      then
         HT_Ops.Reserve_Capacity (HT, HT.Length);
      end if;

      Position.Container := Container'Unrestricted_Access;
   end Insert;

   procedure Insert
     (Container : in out Map;
      Key       : Key_Type;
      New_Item  : Element_Type)
   is
      Position : Cursor;
      pragma Unreferenced (Position);

      Inserted : Boolean;

   begin
      Insert (Container, Key, New_Item, Position, Inserted);

      if Checks and then not Inserted then
         raise Constraint_Error with
           "attempt to insert key already in map";
      end if;
   end Insert;

   --------------
   -- Is_Empty --
   --------------

   function Is_Empty (Container : Map) return Boolean is
   begin
      return Container.HT.Length = 0;
   end Is_Empty;

   -------------
   -- Iterate --
   -------------

   procedure Iterate
     (Container : Map;
      Process   : not null access procedure (Position : Cursor))
   is
      procedure Process_Node (Node : Node_Access; Position : Hash_Type);
      pragma Inline (Process_Node);

      procedure Local_Iterate is
        new HT_Ops.Generic_Iteration_With_Position (Process_Node);

      ------------------
      -- Process_Node --
      ------------------

      procedure Process_Node (Node : Node_Access; Position : Hash_Type) is
      begin
         Process (Cursor'(Container'Unrestricted_Access, Node, Position));
      end Process_Node;

      Busy : With_Busy (Container.HT.TC'Unrestricted_Access);

   --  Start of processing for Iterate

   begin
      Local_Iterate (Container.HT);
   end Iterate;

   function Iterate
     (Container : Map) return Map_Iterator_Interfaces.Forward_Iterator'Class
   is
   begin
      return It : constant Iterator :=
        (Limited_Controlled with Container => Container'Unrestricted_Access)
      do
         Busy (Container.HT.TC'Unrestricted_Access.all);
      end return;
   end Iterate;

   ---------
   -- Key --
   ---------

   function Key (Position : Cursor) return Key_Type is
   begin
      if Checks and then Position.Node = null then
         raise Constraint_Error with
           "Position cursor of function Key equals No_Element";
      end if;

      pragma Assert (Vet (Position), "bad cursor in function Key");

      return Position.Node.Key;
   end Key;

   ------------
   -- Length --
   ------------

   function Length (Container : Map) return Count_Type is
   begin
      return Container.HT.Length;
   end Length;

   ----------
   -- Move --
   ----------

   procedure Move
     (Target : in out Map;
      Source : in out Map)
   is
   begin
      HT_Ops.Move (Target => Target.HT, Source => Source.HT);
   end Move;

   ----------
   -- Next --
   ----------

   function Next (Node : Node_Access) return Node_Access is
   begin
      return Node.Next;
   end Next;

   function Next (Position : Cursor) return Cursor is
      Node    : Node_Access := null;

      Pos : Hash_Type;
      --  Position of cursor's element in the map buckets.
   begin
      if Position.Node = null then
         return No_Element;
      end if;

      pragma Assert (Vet (Position), "bad cursor in function Next");

      --  Initialize to current position, so that HT_Ops.Next can use it
      Pos := Position.Position;

      Node := HT_Ops.Next (Position.Container.HT, Position.Node, Pos);

      if Node = null then
         return No_Element;
      else
         return Cursor'(Position.Container, Node, Pos);
      end if;
   end Next;

   procedure Next (Position : in out Cursor) is
   begin
      Position := Next (Position);
   end Next;

   function Next
     (Object   : Iterator;
      Position : Cursor) return Cursor
   is
   begin
      if Position.Container = null then
         return No_Element;
      end if;

      if Checks and then Position.Container /= Object.Container then
         raise Program_Error with
           "Position cursor of Next designates wrong map";
      end if;

      return Next (Position);
   end Next;

   ----------------------
   -- Pseudo_Reference --
   ----------------------

   function Pseudo_Reference
     (Container : aliased Map'Class) return Reference_Control_Type
   is
      TC : constant Tamper_Counts_Access :=
        Container.HT.TC'Unrestricted_Access;
   begin
      return R : constant Reference_Control_Type := (Controlled with TC) do
         Busy (TC.all);
      end return;
   end Pseudo_Reference;

   -------------------
   -- Query_Element --
   -------------------

   procedure Query_Element
     (Position : Cursor;
      Process  : not null access
                   procedure (Key : Key_Type; Element : Element_Type))
   is
   begin
      if Checks and then Position.Node = null then
         raise Constraint_Error with
           "Position cursor of Query_Element equals No_Element";
      end if;

      pragma Assert (Vet (Position), "bad cursor in Query_Element");

      declare
         M  : Map renames Position.Container.all;
         HT : Hash_Table_Type renames M.HT'Unrestricted_Access.all;
         Lock : With_Lock (HT.TC'Unrestricted_Access);
         K : Key_Type renames Position.Node.Key;
         E : Element_Type renames Position.Node.Element;
      begin
         Process (K, E);
      end;
   end Query_Element;

   ----------
   -- Read --
   ----------

   procedure Read
     (Stream    : not null access Root_Stream_Type'Class;
      Container : out Map)
   is
   begin
      Read_Nodes (Stream, Container.HT);
   end Read;

   procedure Read
     (Stream : not null access Root_Stream_Type'Class;
      Item   : out Cursor)
   is
   begin
      raise Program_Error with "attempt to stream map cursor";
   end Read;

   procedure Read
     (Stream : not null access Root_Stream_Type'Class;
      Item   : out Reference_Type)
   is
   begin
      raise Program_Error with "attempt to stream reference";
   end Read;

   procedure Read
     (Stream : not null access Root_Stream_Type'Class;
      Item   : out Constant_Reference_Type)
   is
   begin
      raise Program_Error with "attempt to stream reference";
   end Read;

   ---------------
   -- Reference --
   ---------------

   function Reference
     (Container : aliased in out Map;
      Position  : Cursor) return Reference_Type
   is
   begin
      if Checks and then Position.Container = null then
         raise Constraint_Error with
           "Position cursor has no element";
      end if;

      if Checks and then Position.Container /= Container'Unrestricted_Access
      then
         raise Program_Error with
           "Position cursor designates wrong map";
      end if;

      pragma Assert
        (Vet (Position),
         "Position cursor in function Reference is bad");

      declare
         HT : Hash_Table_Type renames Container'Unrestricted_Access.all.HT;
         TC : constant Tamper_Counts_Access :=
           HT.TC'Unrestricted_Access;
      begin
         return R : constant Reference_Type :=
           (Element => Position.Node.Element'Access,
            Control => (Controlled with TC))
         do
            Busy (TC.all);
         end return;
      end;
   end Reference;

   function Reference
     (Container : aliased in out Map;
      Key       : Key_Type) return Reference_Type
   is
      HT   : Hash_Table_Type renames Container.HT;
      Node : constant Node_Access := Key_Ops.Find (HT, Key);

   begin
      if Checks and then Node = null then
         raise Constraint_Error with "key not in map";
      end if;

      declare
         TC : constant Tamper_Counts_Access :=
           HT.TC'Unrestricted_Access;
      begin
         return R : constant Reference_Type :=
           (Element => Node.Element'Access,
            Control => (Controlled with TC))
         do
            Busy (TC.all);
         end return;
      end;
   end Reference;

   ---------------
   -- Read_Node --
   ---------------

   function Read_Node
     (Stream : not null access Root_Stream_Type'Class) return Node_Access
   is
      Node : Node_Access := new Node_Type;

   begin
      Key_Type'Read (Stream, Node.Key);
      Element_Type'Read (Stream, Node.Element);
      return Node;

   exception
      when others =>
         Free (Node);
         raise;
   end Read_Node;

   -------------
   -- Replace --
   -------------

   procedure Replace
     (Container : in out Map;
      Key       : Key_Type;
      New_Item  : Element_Type)
   is
      Node : constant Node_Access := Key_Ops.Find (Container.HT, Key);

   begin
      if Checks and then Node = null then
         raise Constraint_Error with
           "attempt to replace key not in map";
      end if;

      TE_Check (Container.HT.TC);

      Node.Key := Key;
      Node.Element := New_Item;
   end Replace;

   ---------------------
   -- Replace_Element --
   ---------------------

   procedure Replace_Element
     (Container : in out Map;
      Position  : Cursor;
      New_Item  : Element_Type)
   is
   begin
      if Checks and then Position.Node = null then
         raise Constraint_Error with
           "Position cursor of Replace_Element equals No_Element";
      end if;

      if Checks and then Position.Container /= Container'Unrestricted_Access
      then
         raise Program_Error with
           "Position cursor of Replace_Element designates wrong map";
      end if;

      TE_Check (Position.Container.HT.TC);

      pragma Assert (Vet (Position), "bad cursor in Replace_Element");

      Position.Node.Element := New_Item;
   end Replace_Element;

   ----------------------
   -- Reserve_Capacity --
   ----------------------

   procedure Reserve_Capacity
     (Container : in out Map;
      Capacity  : Count_Type)
   is
   begin
      HT_Ops.Reserve_Capacity (Container.HT, Capacity);
   end Reserve_Capacity;

   --------------
   -- Set_Next --
   --------------

   procedure Set_Next (Node : Node_Access; Next : Node_Access) is
   begin
      Node.Next := Next;
   end Set_Next;

   --------------------
   -- Update_Element --
   --------------------

   procedure Update_Element
     (Container : in out Map;
      Position  : Cursor;
      Process   : not null access procedure (Key     : Key_Type;
                                             Element : in out Element_Type))
   is
   begin
      if Checks and then Position.Node = null then
         raise Constraint_Error with
           "Position cursor of Update_Element equals No_Element";
      end if;

      if Checks and then Position.Container /= Container'Unrestricted_Access
      then
         raise Program_Error with
           "Position cursor of Update_Element designates wrong map";
      end if;

      pragma Assert (Vet (Position), "bad cursor in Update_Element");

      declare
         HT : Hash_Table_Type renames Container.HT;
         Lock : With_Lock (HT.TC'Unrestricted_Access);
         K : Key_Type renames Position.Node.Key;
         E : Element_Type renames Position.Node.Element;
      begin
         Process (K, E);
      end;
   end Update_Element;

   ---------
   -- Vet --
   ---------

   function Vet (Position : Cursor) return Boolean is
   begin
      if Position.Node = null then
         return Position.Container = null;
      end if;

      if Position.Container = null then
         return False;
      end if;

      if Position.Node.Next = Position.Node then
         return False;
      end if;

      declare
         HT : Hash_Table_Type renames Position.Container.HT;
         X  : Node_Access;

      begin
         if HT.Length = 0 then
            return False;
         end if;

         if HT.Buckets = null
           or else HT.Buckets'Length = 0
         then
            return False;
         end if;

         X := HT.Buckets (Key_Ops.Checked_Index (HT, Position.Node.Key));

         for J in 1 .. HT.Length loop
            if X = Position.Node then
               return True;
            end if;

            if X = null then
               return False;
            end if;

            if X = X.Next then  --  to prevent unnecessary looping
               return False;
            end if;

            X := X.Next;
         end loop;

         return False;
      end;
   end Vet;

   -----------
   -- Write --
   -----------

   procedure Write
     (Stream    : not null access Root_Stream_Type'Class;
      Container : Map)
   is
   begin
      Write_Nodes (Stream, Container.HT);
   end Write;

   procedure Write
     (Stream : not null access Root_Stream_Type'Class;
      Item   : Cursor)
   is
   begin
      raise Program_Error with "attempt to stream map cursor";
   end Write;

   procedure Write
     (Stream : not null access Root_Stream_Type'Class;
      Item   : Reference_Type)
   is
   begin
      raise Program_Error with "attempt to stream reference";
   end Write;

   procedure Write
     (Stream : not null access Root_Stream_Type'Class;
      Item   : Constant_Reference_Type)
   is
   begin
      raise Program_Error with "attempt to stream reference";
   end Write;

   ----------------
   -- Write_Node --
   ----------------

   procedure Write_Node
     (Stream : not null access Root_Stream_Type'Class;
      Node   : Node_Access)
   is
   begin
      Key_Type'Write (Stream, Node.Key);
      Element_Type'Write (Stream, Node.Element);
   end Write_Node;

end Ada.Containers.Hashed_Maps;
