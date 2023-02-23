------------------------------------------------------------------------------
--                                                                          --
--                         GNAT RUN-TIME COMPONENTS                         --
--                                                                          --
--         A D A . S T R I N G S . U N B O U N D E D . T E X T _ I O        --
--                                                                          --
--                                 B o d y                                  --
--                                                                          --
--          Copyright (C) 1997-2023, Free Software Foundation, Inc.         --
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

with Ada.Text_IO; use Ada.Text_IO;

package body Ada.Strings.Unbounded.Text_IO is

   --------------
   -- Get_Line --
   --------------

   function Get_Line return Unbounded_String is
      Buffer : String (1 .. 1000);
      Last   : Natural;
      Str1   : String_Access;
      Str2   : String_Access;
      Result : Unbounded_String;

   begin
      Get_Line (Buffer, Last);
      Str1 := new String'(Buffer (1 .. Last));
      while Last = Buffer'Last loop
         Get_Line (Buffer, Last);
         Str2 := new String (1 .. Str1'Last + Last);
         Str2 (Str1'Range) := Str1.all;
         Str2 (Str1'Last + 1 .. Str2'Last) := Buffer (1 .. Last);
         Free (Str1);
         Str1 := Str2;
      end loop;

      Result.Reference := Str1;
      Result.Last      := Str1'Length;
      return Result;
   end Get_Line;

   function Get_Line (File : Ada.Text_IO.File_Type) return Unbounded_String is
      Buffer : String (1 .. 1000);
      Last   : Natural;
      Str1   : String_Access;
      Str2   : String_Access;
      Result : Unbounded_String;

   begin
      Get_Line (File, Buffer, Last);
      Str1 := new String'(Buffer (1 .. Last));
      while Last = Buffer'Last loop
         Get_Line (File, Buffer, Last);
         Str2 := new String (1 .. Str1'Last + Last);
         Str2 (Str1'Range) := Str1.all;
         Str2 (Str1'Last + 1 .. Str2'Last) := Buffer (1 .. Last);
         Free (Str1);
         Str1 := Str2;
      end loop;

      Result.Reference := Str1;
      Result.Last      := Str1'Length;
      return Result;
   end Get_Line;

   procedure Get_Line (Item : out Unbounded_String) is
   begin
      Get_Line (Current_Input, Item);
   end Get_Line;

   procedure Get_Line
     (File : Ada.Text_IO.File_Type;
      Item : out Unbounded_String)
   is
   begin
      --  We are going to read into the string that is already there and
      --  allocated. Hopefully it is big enough now, if not, we will extend
      --  it in the usual manner using Realloc_For_Chunk.

      --  Make sure we start with at least 80 characters

      if Item.Reference'Last < 80 then
         Realloc_For_Chunk (Item, 80);
      end if;

      --  Loop to read data, filling current string as far as possible.
      --  Item.Last holds the number of characters read so far.

      Item.Last := 0;
      loop
         Get_Line
           (File,
            Item.Reference (Item.Last + 1 .. Item.Reference'Last),
            Item.Last);

         --  If we hit the end of the line before the end of the buffer, then
         --  we are all done, and the result length is properly set.

         if Item.Last < Item.Reference'Last then
            return;
         end if;

         --  If not enough room, double it and keep reading

         Realloc_For_Chunk (Item, Item.Last);
      end loop;
   end Get_Line;

   ---------
   -- Put --
   ---------

   procedure Put (U : Unbounded_String) is
   begin
      Put (U.Reference (1 .. U.Last));
   end Put;

   procedure Put (File : File_Type; U : Unbounded_String) is
   begin
      Put (File, U.Reference (1 .. U.Last));
   end Put;

   --------------
   -- Put_Line --
   --------------

   procedure Put_Line (U : Unbounded_String) is
   begin
      Put_Line (U.Reference (1 .. U.Last));
   end Put_Line;

   procedure Put_Line (File : File_Type; U : Unbounded_String) is
   begin
      Put_Line (File, U.Reference (1 .. U.Last));
   end Put_Line;

end Ada.Strings.Unbounded.Text_IO;
