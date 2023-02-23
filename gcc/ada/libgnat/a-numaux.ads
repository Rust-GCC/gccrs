------------------------------------------------------------------------------
--                                                                          --
--                         GNAT RUN-TIME COMPONENTS                         --
--                                                                          --
--                     A D A . N U M E R I C S . A U X                      --
--                                                                          --
--                                 S p e c                                  --
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

--  This is a backward-compatibility unit, for users of this internal
--  package before the introduction of Aux.Generic_Float.

with Ada.Numerics.Aux_Compat;

package Ada.Numerics.Aux is
   pragma Pure;

   package Aux renames Aux_Compat;

   type Double is new Aux.T;

   subtype T is Double;
   subtype W is Aux.T;

   --  Use the Aux implementation.

   function Sin (X : T) return T
   is (T (Aux.Sin (W (X))));

   function Cos (X : T) return T
   is (T (Aux.Cos (W (X))));

   function Tan (X : T) return T
   is (T (Aux.Tan (W (X))));

   function Exp (X : T) return T
   is (T (Aux.Exp (W (X))));

   function Sqrt (X : T) return T
   is (T (Aux.Sqrt (W (X))));

   function Log (X : T) return T
   is (T (Aux.Log (W (X))));

   function Acos (X : T) return T
   is (T (Aux.Acos (W (X))));

   function Asin (X : T) return T
   is (T (Aux.Asin (W (X))));

   function Atan (X : T) return T
   is (T (Aux.Atan (W (X))));

   function Sinh (X : T) return T
   is (T (Aux.Sinh (W (X))));

   function Cosh (X : T) return T
   is (T (Aux.Cosh (W (X))));

   function Tanh (X : T) return T
   is (T (Aux.Tanh (W (X))));

   function Pow (X, Y : T) return T
   is (T (Aux.Pow (W (X), W (Y))));

end Ada.Numerics.Aux;
