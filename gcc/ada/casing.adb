------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                               C A S I N G                                --
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

with Csets;    use Csets;
with Opt;      use Opt;
with Widechar; use Widechar;

package body Casing is

   ----------------------
   -- Determine_Casing --
   ----------------------

   function Determine_Casing (Ident : Text_Buffer) return Casing_Type is

      All_Lower : Boolean := True;
      --  Set False if upper case letter found

      All_Upper : Boolean := True;
      --  Set False if lower case letter found

      Mixed : Boolean := True;
      --  Set False if exception to mixed case rule found (lower case letter
      --  at start or after underline, or upper case letter elsewhere).

      Decisive : Boolean := False;
      --  Set True if at least one instance of letter not after underline

      After_Und : Boolean := True;
      --  True at start of string, and after an underline character

   begin
      --  A special exception, consider SPARK_Mode to be mixed case

      if Ident = "SPARK_Mode" then
         return Mixed_Case;
      end if;

      --  Proceed with normal determination

      for S in Ident'Range loop
         if Ident (S) = '_' or else Ident (S) = '.' then
            After_Und := True;

         elsif Is_Lower_Case_Letter (Ident (S)) then
            All_Upper := False;

            if not After_Und then
               Decisive := True;
            else
               After_Und := False;
               Mixed := False;
            end if;

         elsif Is_Upper_Case_Letter (Ident (S)) then
            All_Lower := False;

            if not After_Und then
               Decisive := True;
               Mixed := False;
            else
               After_Und := False;
            end if;
         end if;
      end loop;

      --  Now we can figure out the result from the flags we set in that loop

      if All_Lower then
         return All_Lower_Case;

      elsif not Decisive then
         return Unknown;

      elsif All_Upper then
         return All_Upper_Case;

      elsif Mixed then
         return Mixed_Case;

      else
         return Unknown;
      end if;
   end Determine_Casing;

   ----------------
   -- Set_Casing --
   ----------------

   procedure Set_Casing
     (Buf : in out Bounded_String;
      C   : Casing_Type;
      D   : Casing_Type := Mixed_Case)
   is
      Ptr : Natural;

      Actual_Casing : Casing_Type;
      --  Set from C or D as appropriate

      After_Und : Boolean := True;
      --  True at start of string, and after an underline character or after
      --  any other special character that is not a normal identifier char).

   begin
      if C /= Unknown then
         Actual_Casing := C;
      else
         Actual_Casing := D;
      end if;

      Ptr := 1;

      while Ptr <= Buf.Length loop

         --  Wide character. Note that we do nothing with casing in this case.
         --  In Ada 2005 mode, required folding of lower case letters happened
         --  as the identifier was scanned, and we do not attempt any further
         --  messing with case (note that in any case we do not know how to
         --  fold upper case to lower case in wide character mode). We also
         --  do not bother with recognizing punctuation as equivalent to an
         --  underscore. There is nothing functional at this stage in doing
         --  the requested casing operation, beyond folding to upper case
         --  when it is mandatory, which does not involve underscores.

         if Buf.Chars (Ptr) = ASCII.ESC
           or else Buf.Chars (Ptr) = '['
           or else (Upper_Half_Encoding
                     and then Buf.Chars (Ptr) in Upper_Half_Character)
         then
            Skip_Wide (Buf.Chars, Ptr);
            After_Und := False;

         --  Underscore, or non-identifer character (error case)

         elsif Buf.Chars (Ptr) = '_'
           or else not Identifier_Char (Buf.Chars (Ptr))
         then
            After_Und := True;
            Ptr := Ptr + 1;

         --  Lower case letter

         elsif Is_Lower_Case_Letter (Buf.Chars (Ptr)) then
            if Actual_Casing = All_Upper_Case
              or else (After_Und and then Actual_Casing = Mixed_Case)
            then
               Buf.Chars (Ptr) := Fold_Upper (Buf.Chars (Ptr));
            end if;

            After_Und := False;
            Ptr := Ptr + 1;

         --  Upper case letter

         elsif Is_Upper_Case_Letter (Buf.Chars (Ptr)) then
            if Actual_Casing = All_Lower_Case
              or else (not After_Und and then Actual_Casing = Mixed_Case)
            then
               Buf.Chars (Ptr) := Fold_Lower (Buf.Chars (Ptr));
            end if;

            After_Und := False;
            Ptr := Ptr + 1;

         --  Other identifier character (must be digit)

         else
            After_Und := False;
            Ptr := Ptr + 1;
         end if;
      end loop;
   end Set_Casing;

   procedure Set_Casing (C : Casing_Type; D : Casing_Type := Mixed_Case) is
   begin
      Set_Casing (Global_Name_Buffer, C, D);
   end Set_Casing;

end Casing;
