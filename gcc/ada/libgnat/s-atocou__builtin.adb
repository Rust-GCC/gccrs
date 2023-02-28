------------------------------------------------------------------------------
--                                                                          --
--                         GNAT RUN-TIME COMPONENTS                         --
--                                                                          --
--               S Y S T E M . A T O M I C _ C O U N T E R S                --
--                                                                          --
--                                 B o d y                                  --
--                                                                          --
--          Copyright (C) 2011-2023, Free Software Foundation, Inc.         --
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

--  This package implements Atomic_Counter and Atomic_Unsigned operations for
--  platforms where GCC supports __atomic_add_fetch and __atomic_sub_fetch
--  builtins.

with System.Atomic_Primitives; use System.Atomic_Primitives;

package body System.Atomic_Counters is

   function Atomic_Add_Fetch
     (Ptr   : System.Address;
      Val   : Atomic_Unsigned;
      Model : Mem_Model := Seq_Cst)
     return Atomic_Unsigned;
   pragma Import (Intrinsic, Atomic_Add_Fetch, "__atomic_add_fetch");

   function Atomic_Sub_Fetch
     (Ptr   : System.Address;
      Val   : Atomic_Unsigned;
      Model : Mem_Model := Seq_Cst)
     return Atomic_Unsigned;
   pragma Import (Intrinsic, Atomic_Sub_Fetch, "__atomic_sub_fetch");

   ---------------
   -- Decrement --
   ---------------

   procedure Decrement (Item : aliased in out Atomic_Unsigned) is
   begin
      if Atomic_Sub_Fetch (Item'Address, 1) = 0 then
         null;
      end if;
   end Decrement;

   function Decrement (Item : aliased in out Atomic_Unsigned) return Boolean is
   begin
      return Atomic_Sub_Fetch (Item'Address, 1) = 0;
   end Decrement;

   function Decrement (Item : in out Atomic_Counter) return Boolean is
   begin
      return Atomic_Sub_Fetch (Item.Value'Address, 1) = 0;
   end Decrement;

   ---------------
   -- Increment --
   ---------------

   procedure Increment (Item : aliased in out Atomic_Unsigned) is
   begin
      if Atomic_Add_Fetch (Item'Address, 1) = 0 then
         null;
      end if;
   end Increment;

   procedure Increment (Item : in out Atomic_Counter) is
   begin
      if Atomic_Add_Fetch (Item.Value'Address, 1) = 0 then
         null;
      end if;
   end Increment;

   ----------------
   -- Initialize --
   ----------------

   procedure Initialize (Item : out Atomic_Counter) is
   begin
      Item.Value := 1;
   end Initialize;

   ------------
   -- Is_One --
   ------------

   function Is_One (Item : Atomic_Counter) return Boolean is
   begin
      return Item.Value = 1;
   end Is_One;

end System.Atomic_Counters;
