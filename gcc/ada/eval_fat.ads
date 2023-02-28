------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                             E V A L _ F A T                              --
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
-- or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License --
-- for  more details.  You should have  received  a copy of the GNU General --
-- Public License  distributed with GNAT; see file COPYING3.  If not, go to --
-- http://www.gnu.org/licenses for a complete copy of the license.          --
--                                                                          --
-- GNAT was originally developed  by the GNAT team at  New York University. --
-- Extensive contributions were provided by Ada Core Technologies Inc.      --
--                                                                          --
------------------------------------------------------------------------------

--  This package provides for compile-time evaluation of static calls to the
--  floating-point attribute functions. It is the compile-time equivalent of
--  the System.Fat_Gen runtime package. The coding is quite similar, as are
--  the subprogram specs, except that the type is passed as an explicit
--  first parameter (and used via ttypes, to obtain the necessary information
--  about the characteristics of the type for computing the results.

with Types;  use Types;
with Uintp;  use Uintp;
with Urealp; use Urealp;

package Eval_Fat is

   subtype UI is Uint;
   --  The compile time representation of universal integer

   subtype T is Ureal;
   --  The compile time representation of floating-point values

   subtype R is Entity_Id;
   --  The compile time representation of the floating-point root type

   --  The following functions perform the operation implied by their name
   --  which corresponds to the name of the attribute which they compute.
   --  The arguments correspond to the attribute function arguments.

   function Adjacent          (RT : R; X, Towards : T)              return T;

   function Ceiling           (RT : R; X : T)                       return T;

   function Compose           (RT : R; Fraction : T; Exponent : UI) return T;

   function Copy_Sign         (RT : R; Value, Sign : T)             return T;

   function Exponent          (RT : R; X : T)                       return UI;

   function Floor             (RT : R; X : T)                       return T;

   function Fraction          (RT : R; X : T)                       return T;

   function Leading_Part      (RT : R; X : T; Radix_Digits : UI)    return T;

   function Model             (RT : R; X : T)                       return T;

   function Pred              (RT : R; X : T)                       return T;

   function Remainder         (RT : R; X, Y : T)                    return T;

   function Rounding          (RT : R; X : T)                       return T;

   function Scaling           (RT : R; X : T; Adjustment : UI)      return T;

   function Succ              (RT : R; X : T)                       return T;

   function Truncation        (RT : R; X : T)                       return T;

   function Unbiased_Rounding (RT : R; X : T)                       return T;

   --  The following global declarations are used by the Machine attribute

   type Rounding_Mode is (Floor, Ceiling, Round, Round_Even);
   for Rounding_Mode use (0, 1, 2, 3);
   pragma Convention (C, Rounding_Mode);
   --  Used to indicate rounding mode for Machine attribute

   --  The Machine attribute is special, in that it takes an extra argument
   --  indicating the rounding mode, and also an argument Enode that is a
   --  node used to post warnings (e.g. if asked to convert a negative zero
   --  on a machine for which Signed_Zeros is False).

   function Machine
     (RT    : R;
      X     : T;
      Mode  : Rounding_Mode;
      Enode : Node_Id) return T;

   --  WARNING: There is a matching C declaration of this function in urealp.h

   procedure Decompose_Int
     (RT       : R;
      X        : T;
      Fraction : out UI;
      Exponent : out UI;
      Mode     : Rounding_Mode);
   --  Decomposes a floating-point number into fraction and exponent parts.
   --  The Fraction value returned is an integer representing the value
   --  Fraction * Scale, where Scale is the value (Machine_Radix_Value (RT) **
   --  Machine_Mantissa_Value (RT)). The value is obtained by using biased
   --  rounding (halfway cases round away from zero), round to even, a floor
   --  operation or a ceiling operation depending on the setting of Mode (see
   --  corresponding descriptions in Urealp).

end Eval_Fat;
