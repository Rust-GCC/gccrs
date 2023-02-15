------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                I N T E R F A C E S . C . P O I N T E R S                 --
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

with Interfaces.C.Strings; use Interfaces.C.Strings;
with System;               use System;

with Ada.Unchecked_Conversion;

package body Interfaces.C.Pointers is

   type Addr is mod 2 ** System.Parameters.ptr_bits;

   function To_Pointer is new Ada.Unchecked_Conversion (Addr,      Pointer);
   function To_Addr    is new Ada.Unchecked_Conversion (Pointer,   Addr);
   function To_Addr    is new Ada.Unchecked_Conversion (ptrdiff_t, Addr);
   function To_Ptrdiff is new Ada.Unchecked_Conversion (Addr,      ptrdiff_t);

   Elmt_Size : constant ptrdiff_t :=
                 (Element_Array'Component_Size
                   + Storage_Unit - 1) / Storage_Unit;

   subtype Index_Base is Index'Base;

   ---------
   -- "+" --
   ---------

   function "+" (Left : Pointer; Right : ptrdiff_t) return Pointer is
   begin
      if Left = null then
         raise Pointer_Error;
      end if;

      return To_Pointer (To_Addr (Left) + To_Addr (Elmt_Size * Right));
   end "+";

   function "+" (Left : ptrdiff_t; Right : Pointer) return Pointer is
   begin
      if Right = null then
         raise Pointer_Error;
      end if;

      return To_Pointer (To_Addr (Elmt_Size * Left) + To_Addr (Right));
   end "+";

   ---------
   -- "-" --
   ---------

   function "-" (Left : Pointer; Right : ptrdiff_t) return Pointer is
   begin
      if Left = null then
         raise Pointer_Error;
      end if;

      return To_Pointer (To_Addr (Left) - To_Addr (Right * Elmt_Size));
   end "-";

   function "-" (Left : Pointer; Right : Pointer) return ptrdiff_t is
   begin
      if Left = null or else Right = null then
         raise Pointer_Error;
      end if;

      return To_Ptrdiff (To_Addr (Left) - To_Addr (Right)) / Elmt_Size;
   end "-";

   ----------------
   -- Copy_Array --
   ----------------

   procedure Copy_Array
     (Source  : Pointer;
      Target  : Pointer;
      Length  : ptrdiff_t)
   is
      T : Pointer;
      S : Pointer;

   begin
      if Source = null or else Target = null then
         raise Dereference_Error;

      --  Forward copy

      elsif To_Addr (Target) <= To_Addr (Source) then
         T := Target;
         S := Source;
         for J in 1 .. Length loop
            T.all := S.all;
            Increment (T);
            Increment (S);
         end loop;

      --  Backward copy

      else
         T := Target + Length;
         S := Source + Length;
         for J in 1 .. Length loop
            Decrement (T);
            Decrement (S);
            T.all := S.all;
         end loop;
      end if;
   end Copy_Array;

   ---------------------------
   -- Copy_Terminated_Array --
   ---------------------------

   procedure Copy_Terminated_Array
     (Source     : Pointer;
      Target     : Pointer;
      Limit      : ptrdiff_t := ptrdiff_t'Last;
      Terminator : Element := Default_Terminator)
   is
      L : ptrdiff_t;
      S : Pointer := Source;

   begin
      if Source = null or Target = null then
         raise Dereference_Error;
      end if;

      --  Compute array limited length (including the terminator)

      L := 0;
      while L < Limit loop
         L := L + 1;
         exit when S.all = Terminator;
         Increment (S);
      end loop;

      Copy_Array (Source, Target, L);
   end Copy_Terminated_Array;

   ---------------
   -- Decrement --
   ---------------

   procedure Decrement (Ref : in out Pointer) is
   begin
      Ref := Ref - 1;
   end Decrement;

   ---------------
   -- Increment --
   ---------------

   procedure Increment (Ref : in out Pointer) is
   begin
      Ref := Ref + 1;
   end Increment;

   -----------
   -- Value --
   -----------

   function Value
     (Ref        : Pointer;
      Terminator : Element := Default_Terminator) return Element_Array
   is
      P : Pointer;
      L : constant Index_Base := Index'First;
      H : Index_Base;

   begin
      if Ref = null then
         raise Dereference_Error;

      else
         H := L;
         P := Ref;

         loop
            exit when P.all = Terminator;
            H := Index_Base'Succ (H);
            Increment (P);
         end loop;

         declare
            subtype A is Element_Array (L .. H);

            type PA is access A;
            for PA'Size use System.Parameters.ptr_bits;
            function To_PA is new Ada.Unchecked_Conversion (Pointer, PA);

         begin
            return To_PA (Ref).all;
         end;
      end if;
   end Value;

   function Value
     (Ref    : Pointer;
      Length : ptrdiff_t) return Element_Array
   is
      L : Index_Base;
      H : Index_Base;

   begin
      if Ref = null then
         raise Dereference_Error;

      --  For length zero, we need to return a null slice, but we can't make
      --  the bounds of this slice Index'First, since this could cause a
      --  Constraint_Error if Index'First = Index'Base'First.

      elsif Length <= 0 then
         declare
            pragma Warnings (Off); -- kill warnings since X not assigned
            X : Element_Array (Index'Succ (Index'First) .. Index'First);
            pragma Warnings (On);

         begin
            return X;
         end;

      --  Normal case (length non-zero)

      else
         L := Index'First;
         H := Index'Val (Index'Pos (Index'First) + Length - 1);

         declare
            subtype A is Element_Array (L .. H);

            type PA is access A;
            for PA'Size use System.Parameters.ptr_bits;
            function To_PA is new Ada.Unchecked_Conversion (Pointer, PA);

         begin
            return To_PA (Ref).all;
         end;
      end if;
   end Value;

   --------------------
   -- Virtual_Length --
   --------------------

   function Virtual_Length
     (Ref        : Pointer;
      Terminator : Element := Default_Terminator) return ptrdiff_t
   is
      P : Pointer;
      C : ptrdiff_t;

   begin
      if Ref = null then
         raise Dereference_Error;

      else
         C := 0;
         P := Ref;

         while P.all /= Terminator loop
            C := C + 1;
            Increment (P);
         end loop;

         return C;
      end if;
   end Virtual_Length;

end Interfaces.C.Pointers;
