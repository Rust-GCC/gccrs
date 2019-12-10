------------------------------------------------------------------------------
--                                                                          --
--                         GNAT RUN-TIME COMPONENTS                         --
--                                                                          --
--                            G N A T . L I S T S                           --
--                                                                          --
--                                 S p e c                                  --
--                                                                          --
--             Copyright (C) 2018-2019, Free Software Foundation, Inc.      --
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
-- GNAT was originally developed  by the GNAT team at  New York University. --
-- Extensive contributions were provided by Ada Core Technologies Inc.      --
--                                                                          --
------------------------------------------------------------------------------

pragma Compiler_Unit_Warning;

package GNAT.Lists is

   ------------------------
   -- Doubly_Linked_List --
   ------------------------

   --  The following package offers a doubly linked list abstraction with the
   --  following characteristics:
   --
   --    * Creation of multiple instances, of different sizes
   --    * Iterable elements
   --
   --  The following use pattern must be employed with this list:
   --
   --    List : Doubly_Linked_List := Create;
   --
   --    <various operations>
   --
   --    Destroy (List);
   --
   --  The destruction of the list reclaims all storage occupied by it.

   generic
      type Element_Type is private;

      with function "="
        (Left  : Element_Type;
         Right : Element_Type) return Boolean;

      with procedure Destroy_Element (Elem : in out Element_Type);
      --  Element destructor

   package Doubly_Linked_Lists is

      ---------------------
      -- List operations --
      ---------------------

      type Doubly_Linked_List is private;
      Nil : constant Doubly_Linked_List;

      --  The following exception is raised when the list is empty, and an
      --  attempt is made to delete an element from it.

      List_Empty : exception;

      procedure Append
        (L    : Doubly_Linked_List;
         Elem : Element_Type);
      --  Insert element Elem at the end of list L. This action will raise
      --  Iterated if the list has outstanding iterators.

      function Contains
        (L    : Doubly_Linked_List;
         Elem : Element_Type) return Boolean;
      --  Determine whether list L contains element Elem

      function Create return Doubly_Linked_List;
      --  Create a new list

      procedure Delete
        (L    : Doubly_Linked_List;
         Elem : Element_Type);
      --  Delete element Elem from list L. The routine has no effect if Elem is
      --  not present. This action will raise
      --
      --    * List_Empty if the list is empty.
      --    * Iterated if the list has outstanding iterators.

      procedure Delete_First (L : Doubly_Linked_List);
      --  Delete an element from the start of list L. This action will raise
      --
      --    * List_Empty if the list is empty.
      --    * Iterated if the list has outstanding iterators.

      procedure Delete_Last (L : Doubly_Linked_List);
      --  Delete an element from the end of list L. This action will raise
      --
      --    * List_Empty if the list is empty.
      --    * Iterated if the list has outstanding iterators.

      procedure Destroy (L : in out Doubly_Linked_List);
      --  Destroy the contents of list L. This routine must be called at the
      --  end of a list's lifetime. This action will raise Iterated if the
      --  list has outstanding iterators.

      function Equal
        (Left  : Doubly_Linked_List;
         Right : Doubly_Linked_List) return Boolean;
      --  Determine whether lists Left and Right have the same characteristics
      --  and contain the same elements.

      function First (L : Doubly_Linked_List) return Element_Type;
      --  Obtain an element from the start of list L. This action will raise
      --  List_Empty if the list is empty.

      procedure Insert_After
        (L     : Doubly_Linked_List;
         After : Element_Type;
         Elem  : Element_Type);
      --  Insert new element Elem after element After in list L. The routine
      --  has no effect if After is not present. This action will raise
      --  Iterated if the list has outstanding iterators.

      procedure Insert_Before
        (L      : Doubly_Linked_List;
         Before : Element_Type;
         Elem   : Element_Type);
      --  Insert new element Elem before element Before in list L. The routine
      --  has no effect if After is not present. This action will raise
      --  Iterated if the list has outstanding iterators.

      function Is_Empty (L : Doubly_Linked_List) return Boolean;
      --  Determine whether list L is empty

      function Last (L : Doubly_Linked_List) return Element_Type;
      --  Obtain an element from the end of list L. This action will raise
      --  List_Empty if the list is empty.

      procedure Prepend
        (L    : Doubly_Linked_List;
         Elem : Element_Type);
      --  Insert element Elem at the start of list L. This action will raise
      --  Iterated if the list has outstanding iterators.

      function Present (L : Doubly_Linked_List) return Boolean;
      --  Determine whether list L exists

      procedure Replace
        (L        : Doubly_Linked_List;
         Old_Elem : Element_Type;
         New_Elem : Element_Type);
      --  Replace old element Old_Elem with new element New_Elem in list L. The
      --  routine has no effect if Old_Elem is not present. This action will
      --  raise Iterated if the list has outstanding iterators.

      function Size (L : Doubly_Linked_List) return Natural;
      --  Obtain the number of elements in list L

      -------------------------
      -- Iterator operations --
      -------------------------

      --  The following type represents an element iterator. An iterator locks
      --  all mutation operations, and ulocks them once it is exhausted. The
      --  iterator must be used with the following pattern:
      --
      --    Iter := Iterate (My_List);
      --    while Has_Next (Iter) loop
      --       Next (Iter, Element);
      --    end loop;
      --
      --  It is possible to advance the iterator by using Next only, however
      --  this risks raising Iterator_Exhausted.

      type Iterator is private;

      function Has_Next (Iter : Iterator) return Boolean;
      --  Determine whether iterator Iter has more elements to examine. If the
      --  iterator has been exhausted, restore all mutation functionality of
      --  the associated list.

      function Iterate (L : Doubly_Linked_List) return Iterator;
      --  Obtain an iterator over the elements of list L. This action locks all
      --  mutation functionality of the associated list.

      procedure Next
        (Iter : in out Iterator;
         Elem : out Element_Type);
      --  Return the current element referenced by iterator Iter and advance
      --  to the next available element. If the iterator has been exhausted
      --  and further attempts are made to advance it, this routine restores
      --  mutation functionality of the associated list, and then raises
      --  Iterator_Exhausted.

   private
      --  The following type represents a list node

      type Node;
      type Node_Ptr is access all Node;
      type Node is record
         Elem : Element_Type;

         Next : Node_Ptr := null;
         Prev : Node_Ptr := null;
      end record;

      --  The following type represents a list

      type Doubly_Linked_List_Attributes is record
         Elements : Natural := 0;
         --  The number of elements in the list

         Iterators : Natural := 0;
         --  Number of outstanding iterators

         Nodes : aliased Node;
         --  The dummy head of the list
      end record;

      type Doubly_Linked_List is access all Doubly_Linked_List_Attributes;
      Nil : constant Doubly_Linked_List := null;

      --  The following type represents an element iterator

      type Iterator is record
         Curr_Nod : Node_Ptr := null;
         --  Reference to the current node being examined. The invariant of the
         --  iterator requires that this field always points to a valid node. A
         --  value of null indicates that the iterator is exhausted.

         List : Doubly_Linked_List := null;
         --  Reference to the associated list
      end record;
   end Doubly_Linked_Lists;

end GNAT.Lists;
