------------------------------------------------------------------------------
--                                                                          --
--                         GNAT LIBRARY COMPONENTS                          --
--                                                                          --
--                  ADA.CONTAINERS.BOUNDED_PRIORITY_QUEUES                  --
--                                                                          --
--                                 S p e c                                  --
--                                                                          --
--          Copyright (C) 2011-2023, Free Software Foundation, Inc.         --
--                                                                          --
-- This specification is derived from the Ada Reference Manual for use with --
-- GNAT. The copyright notice above, and the license provisions that follow --
-- apply solely to the  contents of the part following the private keyword. --
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

with System;

with Ada.Containers.Synchronized_Queue_Interfaces;
with Ada.Containers.Bounded_Doubly_Linked_Lists;

generic
   with package Queue_Interfaces is
     new Ada.Containers.Synchronized_Queue_Interfaces (<>);

   type Queue_Priority is private;

   with function Get_Priority
     (Element : Queue_Interfaces.Element_Type) return Queue_Priority is <>;

   with function Before
     (Left, Right : Queue_Priority) return Boolean is <>;

   Default_Capacity : Count_Type;
   Default_Ceiling  : System.Any_Priority := System.Priority'Last;

package Ada.Containers.Bounded_Priority_Queues with
  SPARK_Mode => Off
is
   pragma Annotate (CodePeer, Skip_Analysis);
   pragma Preelaborate;

   package Implementation is

      --  All identifiers in this unit are implementation defined

      pragma Implementation_Defined;

      type List_Type (Capacity : Count_Type) is tagged limited private;

      procedure Enqueue
        (List     : in out List_Type;
         New_Item : Queue_Interfaces.Element_Type);

      procedure Dequeue
        (List    : in out List_Type;
         Element : out Queue_Interfaces.Element_Type);

      procedure Dequeue
        (List     : in out List_Type;
         At_Least : Queue_Priority;
         Element  : in out Queue_Interfaces.Element_Type;
         Success  : out Boolean);

      function First_Element
        (List : List_Type) return Queue_Interfaces.Element_Type;

      function Length (List : List_Type) return Count_Type;

      function Max_Length (List : List_Type) return Count_Type;

   private

      --  We need a better data structure here, such as a proper heap.  ???

      pragma Warnings (Off);
      --  Otherwise, we get warnings for the uninitialized variable in Insert
      --  in Ada.Containers.Bounded_Doubly_Linked_Lists.
      package List_Types is new Bounded_Doubly_Linked_Lists
        (Element_Type => Queue_Interfaces.Element_Type,
         "="          => Queue_Interfaces."=");
      pragma Warnings (On);

      type List_Type (Capacity : Count_Type) is tagged limited record
         Container  : List_Types.List (Capacity);
         Max_Length : Count_Type := 0;
      end record;

   end Implementation;

   protected type Queue
     (Capacity : Count_Type := Default_Capacity;
      Ceiling  : System.Any_Priority := Default_Ceiling)
   with
     Priority => Ceiling
   is new Queue_Interfaces.Queue with

      overriding entry Enqueue (New_Item : Queue_Interfaces.Element_Type);

      overriding entry Dequeue (Element : out Queue_Interfaces.Element_Type);

      --  The priority queue operation Dequeue_Only_High_Priority had been a
      --  protected entry in early drafts of AI05-0159, but it was discovered
      --  that that operation as specified was not in fact implementable. The
      --  operation was changed from an entry to a protected procedure per the
      --  ARG meeting in Edinburgh (June 2011), with a different signature and
      --  semantics.

      procedure Dequeue_Only_High_Priority
        (At_Least : Queue_Priority;
         Element  : in out Queue_Interfaces.Element_Type;
         Success  : out Boolean);

      overriding function Current_Use return Count_Type;

      overriding function Peak_Use return Count_Type;

   private
      List : Implementation.List_Type (Capacity);
   end Queue;

end Ada.Containers.Bounded_Priority_Queues;
