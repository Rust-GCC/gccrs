------------------------------------------------------------------------------
--                                                                          --
--                         GNAT RUN-TIME COMPONENTS                         --
--                                                                          --
--                      S Y S T E M . I M G _ C H A R                       --
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

--  Character'Image

package System.Img_Char is
   pragma Pure;

   procedure Image_Character
     (V : Character;
      S : in out String;
      P : out Natural);
   --  Computes Character'Image (V) and stores the result in S (1 .. P)
   --  setting the resulting value of P. The caller guarantees that S is
   --  long enough to hold the result, and that S'First is 1.

   procedure Image_Character_05
     (V : Character;
      S : in out String;
      P : out Natural);
   --  Computes Character'Image (V) and stores the result in S (1 .. P)
   --  setting the resulting value of P. The caller guarantees that S is
   --  long enough to hold the result, and that S'First is 1. This version
   --  is for use in Ada 2005 and beyond, where soft hyphen is a non-graphic
   --  and results in "SOFT_HYPHEN" as the output.

end System.Img_Char;
