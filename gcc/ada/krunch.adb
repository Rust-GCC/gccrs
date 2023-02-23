------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                               K R U N C H                                --
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

procedure Krunch
  (Buffer        : in out String;
   Len           : in out Natural;
   Maxlen        : Natural;
   No_Predef     : Boolean)
is
   pragma Assert (Buffer'First = 1);
   --  This is a documented requirement; the assert turns off index warnings

   B1       : Character renames Buffer (1);
   Curlen   : Natural;
   Krlen    : Natural;
   Num_Seps : Natural;
   Startloc : Natural;
   J        : Natural;

begin
   --  Deal with special predefined children cases. Startloc is the first
   --  location for the krunch, set to 1, except for the predefined children
   --  case, where it is set to 3, to start after the standard prefix.

   if No_Predef then
      Startloc := 1;
      Curlen := Len;
      Krlen := Maxlen;

   elsif Len >= 18
     and then Buffer (1 .. 17) = "ada-wide_text_io-"
   then
      Startloc := 3;
      Buffer (2 .. 5) := "-wt-";
      Buffer (6 .. Len - 12) := Buffer (18 .. Len);
      Curlen := Len - 12;
      Krlen  := 8;

   elsif Len >= 23
     and then Buffer (1 .. 22) = "ada-wide_wide_text_io-"
   then
      Startloc := 3;
      Buffer (2 .. 5) := "-zt-";
      Buffer (6 .. Len - 17) := Buffer (23 .. Len);
      Curlen := Len - 17;
      Krlen := 8;

   elsif Len >= 27
     and then Buffer (1 .. 27) = "ada-long_long_long_integer_"
   then
      Startloc := 3;
      Buffer (2 .. Len - 2) := Buffer (4 .. Len);
      Buffer (18 .. Len - 10) := Buffer (26 .. Len - 2);
      Curlen := Len - 10;
      Krlen := 8;

   elsif Len >= 4 and then Buffer (1 .. 4) = "ada-" then
      Startloc := 3;
      Buffer (2 .. Len - 2) := Buffer (4 .. Len);
      Curlen := Len - 2;
      Krlen  := 8;

   elsif Len >= 5 and then Buffer (1 .. 5) = "gnat-" then
      Startloc := 3;
      Buffer (2 .. Len - 3) := Buffer (5 .. Len);
      Curlen := Len - 3;
      Krlen  := 8;

   elsif Len >= 7 and then Buffer (1 .. 7) = "system-" then
      Startloc := 3;
      Buffer (2 .. Len - 5) := Buffer (7 .. Len);
      Curlen := Len - 5;
      if (Curlen >= 3 and then Buffer (Curlen - 2 .. Curlen) = "128")
        or else (Len >= 9 and then
                    (Buffer (3 .. 9) = "exn_lll"
                      or else Buffer (3 .. 9) = "exp_lll"
                      or else Buffer (3 .. 9) = "img_lll"
                      or else Buffer (3 .. 9) = "val_lll"
                      or else Buffer (3 .. 9) = "wid_lll"))
           or else (Curlen = 10 and then Buffer (3 .. 6) = "pack")
      then
         if Len >= 15 and then Buffer (3 .. 15) = "compare_array" then
            Buffer (3 .. 4) := "ca";
            Buffer (5 .. Curlen - 11) := Buffer (16 .. Curlen);
            Curlen := Curlen - 11;
         end if;
         Krlen := 9;
      else
         Krlen := 8;
      end if;

   elsif Len >= 11 and then Buffer (1 .. 11) = "interfaces-" then
      Startloc := 3;
      Buffer (2 .. Len - 9) := Buffer (11 .. Len);
      Curlen := Len - 9;

      --  Only fully krunch historical units. For new units, simply use
      --  the 'i-' prefix instead of 'interfaces-'. Packages Interfaces.C
      --  and Interfaces.Cobol are already in the right form. Package
      --  Interfaces.Definitions is krunched for backward compatibility.

      if        (Curlen >  3 and then Buffer (3 ..  4) = "c-")
        or else (Curlen >  3 and then Buffer (3 ..  4) = "c_")
        or else (Curlen = 13 and then Buffer (3 .. 13) = "definitions")
        or else (Curlen =  9 and then Buffer (3 ..  9) = "fortran")
        or else (Curlen = 16 and then Buffer (3 .. 16) = "packed_decimal")
        or else (Curlen >  8 and then Buffer (3 ..  9) = "vxworks")
        or else (Curlen >  5 and then Buffer (3 ..  6) = "java")
      then
         Krlen := 8;
      else
         Krlen := Maxlen;
      end if;

   --  For the renamings in the obsolescent section, we also force krunching
   --  to 8 characters, but no other special processing is required here.
   --  Note that text_io and calendar are already short enough anyway.

   elsif     (Len =  9 and then Buffer (1 ..  9) = "direct_io")
     or else (Len = 10 and then Buffer (1 .. 10) = "interfaces")
     or else (Len = 13 and then Buffer (1 .. 13) = "io_exceptions")
     or else (Len = 12 and then Buffer (1 .. 12) = "machine_code")
     or else (Len = 13 and then Buffer (1 .. 13) = "sequential_io")
     or else (Len = 20 and then Buffer (1 .. 20) = "unchecked_conversion")
     or else (Len = 22 and then Buffer (1 .. 22) = "unchecked_deallocation")
   then
      Startloc := 1;
      Krlen    := 8;
      Curlen   := Len;

   --  Special case of a child unit whose parent unit is a single letter that
   --  is A, G, I, or S. In order to prevent confusion with krunched names
   --  of predefined units use a tilde rather than a minus as the second
   --  character of the file name.

   elsif Len > 1
     and then Buffer (2) = '-'
     and then (B1 = 'a' or else B1 = 'g' or else B1 = 'i' or else B1 = 's')
     and then Len <= Maxlen
   then
      Buffer (2) := '~';
      return;

   --  Normal case, not a predefined file

   else
      Startloc := 1;
      Curlen   := Len;
      Krlen    := Maxlen;
   end if;

   --  Immediate return if file name is short enough now

   if Curlen <= Krlen then
      Len := Curlen;
      return;
   end if;

   --  If string contains Wide_Wide, replace by a single z

   J := Startloc;
   while J <= Curlen - 8 loop
      if Buffer (J .. J + 8) = "wide_wide"
        and then (J = Startloc
                    or else Buffer (J - 1) = '-'
                    or else Buffer (J - 1) = '_')
        and then (J + 8 = Curlen
                    or else Buffer (J + 9) = '-'
                    or else Buffer (J + 9) = '_')
      then
         Buffer (J) := 'z';
         Buffer (J + 1 .. Curlen - 8) := Buffer (J + 9 .. Curlen);
         Curlen := Curlen - 8;
      end if;

      J := J + 1;
   end loop;

   --  For now, refuse to krunch a name that contains an ESC character (wide
   --  character sequence) since it's too much trouble to do this right ???

   for J in 1 .. Curlen loop
      if Buffer (J) = ASCII.ESC then
         return;
      end if;
   end loop;

   --  Count number of separators (minus signs and underscores) and for now
   --  replace them by spaces. We keep them around till the end to control
   --  the krunching process, and then we eliminate them as the last step

   Num_Seps := 0;
   for J in Startloc .. Curlen loop
      if Buffer (J) = '-' or else Buffer (J) = '_' then
         Buffer (J) := ' ';
         Num_Seps := Num_Seps + 1;
      end if;
   end loop;

   --  Now we do the one character at a time krunch till we are short enough

   while Curlen - Num_Seps > Krlen loop
      declare
         Long_Length : Natural := 0;
         Long_Last   : Natural := 0;
         Piece_Start : Natural;
         Ptr         : Natural;

      begin
         Ptr := Startloc;

         --  Loop through pieces to find longest piece

         while Ptr <= Curlen loop
            Piece_Start := Ptr;

            --  Loop through characters in one piece of name

            while Ptr <= Curlen and then Buffer (Ptr) /= ' ' loop
               Ptr := Ptr + 1;
            end loop;

            if Ptr - Piece_Start > Long_Length then
               Long_Length := Ptr - Piece_Start;
               Long_Last := Ptr - 1;
            end if;

            Ptr := Ptr + 1;
         end loop;

         --  Remove last character of longest piece

         if Long_Last < Curlen then
            Buffer (Long_Last .. Curlen - 1) :=
              Buffer (Long_Last + 1 .. Curlen);
         end if;

         Curlen := Curlen - 1;
      end;
   end loop;

   --  Final step, remove the spaces

   Len := 0;

   for J in 1 .. Curlen loop
      if Buffer (J) /= ' ' then
         Len := Len + 1;
         Buffer (Len) := Buffer (J);
      end if;
   end loop;

   return;
end Krunch;
