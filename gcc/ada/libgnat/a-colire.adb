------------------------------------------------------------------------------
--                                                                          --
--                         GNAT RUN-TIME COMPONENTS                         --
--                                                                          --
--             A D A . C O M M A N D _ L I N E . R E M O V E                --
--                                                                          --
--                                 B o d y                                  --
--                                                                          --
--          Copyright (C) 1999-2023, Free Software Foundation, Inc.         --
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

package body Ada.Command_Line.Remove is

   -----------------------
   -- Local Subprograms --
   -----------------------

   procedure Initialize;
   --  Initialize the Remove_Count and Remove_Args variables

   ----------------
   -- Initialize --
   ----------------

   procedure Initialize is
   begin
      if Remove_Args = null then
         Remove_Count := Argument_Count;
         Remove_Args := new Arg_Nums (1 .. Argument_Count);

         for J in Remove_Args'Range loop
            Remove_Args (J) := J;
         end loop;
      end if;
   end Initialize;

   ---------------------
   -- Remove_Argument --
   ---------------------

   procedure Remove_Argument (Number : Positive) is
   begin
      Initialize;

      if Number > Remove_Count then
         raise Constraint_Error;
      end if;

      Remove_Count := Remove_Count - 1;

      for J in Number .. Remove_Count loop
         Remove_Args (J) := Remove_Args (J + 1);
      end loop;
   end Remove_Argument;

   procedure Remove_Argument (Argument : String) is
   begin
      for J in reverse 1 .. Argument_Count loop
         if Argument = Ada.Command_Line.Argument (J) then
            Remove_Argument (J);
         end if;
      end loop;
   end Remove_Argument;

   ----------------------
   -- Remove_Arguments --
   ----------------------

   procedure Remove_Arguments (From : Positive; To : Natural) is
   begin
      Initialize;

      if From > Remove_Count
        or else To > Remove_Count
      then
         raise Constraint_Error;
      end if;

      if To >= From then
         Remove_Count := Remove_Count - (To - From + 1);

         for J in From .. Remove_Count loop
            Remove_Args (J) := Remove_Args (J + (To - From + 1));
         end loop;
      end if;
   end Remove_Arguments;

   procedure Remove_Arguments (Argument_Prefix : String) is
   begin
      for J in reverse 1 .. Argument_Count loop
         declare
            Arg : constant String := Argument (J);

         begin
            if Arg'Length >= Argument_Prefix'Length
              and then Arg (1 .. Argument_Prefix'Length) = Argument_Prefix
            then
               Remove_Argument (J);
            end if;
         end;
      end loop;
   end Remove_Arguments;

end Ada.Command_Line.Remove;
