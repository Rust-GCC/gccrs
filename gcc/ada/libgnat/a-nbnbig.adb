------------------------------------------------------------------------------
--                                                                          --
--                         GNAT RUN-TIME COMPONENTS                         --
--                                                                          --
--               ADA.NUMERICS.BIG_NUMBERS.BIG_INTEGERS_GHOST                --
--                                                                          --
--                                 B o d y                                  --
--                                                                          --
--            Copyright (C) 2021-2023, Free Software Foundation, Inc.       --
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

--  This body is provided as a work-around for a GNAT compiler bug, as GNAT
--  currently does not compile instantiations of the spec with imported ghost
--  generics for packages Signed_Conversions and Unsigned_Conversions.

--  Ghost code in this unit is meant for analysis only, not for run-time
--  checking. This is enforced by setting the assertion policy to Ignore.

pragma Assertion_Policy (Ghost => Ignore);

package body Ada.Numerics.Big_Numbers.Big_Integers_Ghost with
   SPARK_Mode => Off
is

   package body Signed_Conversions with
     SPARK_Mode => Off
   is

      function To_Big_Integer (Arg : Int) return Valid_Big_Integer is
      begin
         raise Program_Error;
         return (null record);
      end To_Big_Integer;

      function From_Big_Integer (Arg : Valid_Big_Integer) return Int is
      begin
         raise Program_Error;
         return 0;
      end From_Big_Integer;

   end Signed_Conversions;

   package body Unsigned_Conversions with
     SPARK_Mode => Off
   is

      function To_Big_Integer (Arg : Int) return Valid_Big_Integer is
      begin
         raise Program_Error;
         return (null record);
      end To_Big_Integer;

      function From_Big_Integer (Arg : Valid_Big_Integer) return Int is
      begin
         raise Program_Error;
         return 0;
      end From_Big_Integer;

   end Unsigned_Conversions;

end Ada.Numerics.Big_Numbers.Big_Integers_Ghost;
