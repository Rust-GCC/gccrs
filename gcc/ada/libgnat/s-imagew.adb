------------------------------------------------------------------------------
--                                                                          --
--                         GNAT RUN-TIME COMPONENTS                         --
--                                                                          --
--                       S Y S T E M . I M A G E _ W                        --
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

package body System.Image_W is

   -----------------------------
   -- Set_Image_Width_Integer --
   -----------------------------

   procedure Set_Image_Width_Integer
     (V : Int;
      W : Integer;
      S : out String;
      P : in out Natural)
   is
      Start : Natural;

   begin
      --  Positive case can just use the unsigned circuit directly

      if V >= 0 then
         Set_Image_Width_Unsigned (Uns (V), W, S, P);

      --  Negative case has to set a minus sign. Note also that we have to be
      --  careful not to generate overflow with the largest negative number.

      else
         P := P + 1;
         S (P) := ' ';
         Start := P;

         declare
            pragma Suppress (Overflow_Check);
            pragma Suppress (Range_Check);
         begin
            Set_Image_Width_Unsigned (Uns (-V), W - 1, S, P);
         end;

         --  Set minus sign in last leading blank location. Because of the
         --  code above, there must be at least one such location.

         while S (Start + 1) = ' ' loop
            Start := Start + 1;
         end loop;

         S (Start) := '-';
      end if;

   end Set_Image_Width_Integer;

   ------------------------------
   -- Set_Image_Width_Unsigned --
   ------------------------------

   procedure Set_Image_Width_Unsigned
     (V : Uns;
      W : Integer;
      S : out String;
      P : in out Natural)
   is
      Start : constant Natural := P;
      F, T  : Natural;

      procedure Set_Digits (T : Uns);
      --  Set digits of absolute value of T

      ----------------
      -- Set_Digits --
      ----------------

      procedure Set_Digits (T : Uns) is
      begin
         if T >= 10 then
            Set_Digits (T / 10);
            pragma Assert (P >= (S'First - 1) and P < S'Last and
                           P < Natural'Last);
            --  No check is done since, as documented in the specification,
            --  the caller guarantees that S is long enough to hold the result.
            P := P + 1;
            S (P) := Character'Val (T mod 10 + Character'Pos ('0'));

         else
            pragma Assert (P >= (S'First - 1) and P < S'Last and
                           P < Natural'Last);
            --  No check is done since, as documented in the specification,
            --  the caller guarantees that S is long enough to hold the result.
            P := P + 1;
            S (P) := Character'Val (T + Character'Pos ('0'));
         end if;
      end Set_Digits;

   --  Start of processing for Set_Image_Width_Unsigned

   begin
      Set_Digits (V);

      --  Add leading spaces if required by width parameter

      if P - Start < W then
         F := P;
         P := P + (W - (P - Start));
         T := P;

         while F > Start loop
            pragma Assert (T >= S'First and T <= S'Last and
                           F >= S'First and F <= S'Last);
            --  No check is done since, as documented in the specification,
            --  the caller guarantees that S is long enough to hold the result.
            S (T) := S (F);
            T := T - 1;
            F := F - 1;
         end loop;

         for J in Start + 1 .. T loop
            pragma Assert (J >= S'First and J <= S'Last);
            --  No check is done since, as documented in the specification,
            --  the caller guarantees that S is long enough to hold the result.
            S (J) := ' ';
         end loop;
      end if;

   end Set_Image_Width_Unsigned;

end System.Image_W;
