------------------------------------------------------------------------------
--                                                                          --
--                         GNAT RUN-TIME COMPONENTS                         --
--                                                                          --
--                        S Y S T E M . E X P I N T                         --
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

--  Integer exponentiation (checks on)

--  Preconditions, postconditions, ghost code, loop invariants and assertions
--  in this unit are meant for analysis only, not for run-time checking, as it
--  would be too costly otherwise. This is enforced by setting the assertion
--  policy to Ignore.

pragma Assertion_Policy (Pre            => Ignore,
                         Post           => Ignore,
                         Ghost          => Ignore,
                         Loop_Invariant => Ignore,
                         Assert         => Ignore);

with System.Expont;

package System.Exp_Int
  with SPARK_Mode
is

   package Expont_Integer is new Expont (Integer);

   function Exp_Integer (Left : Integer; Right : Natural) return Integer
     renames Expont_Integer.Expon;

end System.Exp_Int;
