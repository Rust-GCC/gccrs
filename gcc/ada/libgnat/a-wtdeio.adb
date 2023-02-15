------------------------------------------------------------------------------
--                                                                          --
--                         GNAT RUN-TIME COMPONENTS                         --
--                                                                          --
--           A D A . W I D E _ T E X T _ I O . D E C I M A L _ I O          --
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

with Ada.Wide_Text_IO.Decimal_Aux;
with System.Img_Decimal_32;  use System.Img_Decimal_32;
with System.Img_Decimal_64;  use System.Img_Decimal_64;
with System.Val_Decimal_32;  use System.Val_Decimal_32;
with System.Val_Decimal_64;  use System.Val_Decimal_64;
with System.WCh_Con; use System.WCh_Con;
with System.WCh_WtS; use System.WCh_WtS;

package body Ada.Wide_Text_IO.Decimal_IO is

   subtype Int32 is Interfaces.Integer_32;
   subtype Int64 is Interfaces.Integer_64;

   package Aux32 is new
     Ada.Wide_Text_IO.Decimal_Aux
       (Int32,
        Scan_Decimal32,
        Set_Image_Decimal32);

   package Aux64 is new
     Ada.Wide_Text_IO.Decimal_Aux
       (Int64,
        Scan_Decimal64,
        Set_Image_Decimal64);

   Need64 : constant Boolean := Num'Size > 32;
   --  Throughout this generic body, we distinguish between the case where type
   --  Int32 is acceptable and where type Int64 is needed. This Boolean is used
   --  to test for these cases and since it is a constant, only code for the
   --  relevant case will be included in the instance.

   Scale : constant Integer := Num'Scale;

   ---------
   -- Get --
   ---------

   procedure Get
     (File  : File_Type;
      Item  : out Num;
      Width : Field := 0)
   is
      pragma Unsuppress (Range_Check);

   begin
      if Need64 then
         Item := Num'Fixed_Value (Aux64.Get (File, Width, Scale));
      else
         Item := Num'Fixed_Value (Aux32.Get (File, Width, Scale));
      end if;

   exception
      when Constraint_Error => raise Data_Error;
   end Get;

   procedure Get
     (Item  : out Num;
      Width : Field := 0)
   is
   begin
      Get (Current_In, Item, Width);
   end Get;

   procedure Get
     (From : Wide_String;
      Item : out Num;
      Last : out Positive)
   is
      pragma Unsuppress (Range_Check);

      S : constant String := Wide_String_To_String (From, WCEM_Upper);
      --  String on which we do the actual conversion. Note that the method
      --  used for wide character encoding is irrelevant, since if there is
      --  a character outside the Standard.Character range then the call to
      --  Aux.Gets will raise Data_Error in any case.

   begin
      if Need64 then
         Item := Num'Fixed_Value (Aux64.Gets (S, Last, Scale));
      else
         Item := Num'Fixed_Value (Aux32.Gets (S, Last, Scale));
      end if;

   exception
      when Constraint_Error => raise Data_Error;
   end Get;

   ---------
   -- Put --
   ---------

   procedure Put
     (File : File_Type;
      Item : Num;
      Fore : Field := Default_Fore;
      Aft  : Field := Default_Aft;
      Exp  : Field := Default_Exp)
   is
   begin
      if Need64 then
         Aux64.Put
           (File, Int64'Integer_Value (Item), Fore, Aft, Exp, Scale);
      else
         Aux32.Put
           (File, Int32'Integer_Value (Item), Fore, Aft, Exp, Scale);
      end if;
   end Put;

   procedure Put
     (Item : Num;
      Fore : Field := Default_Fore;
      Aft  : Field := Default_Aft;
      Exp  : Field := Default_Exp)
   is
   begin
      Put (Current_Out, Item, Fore, Aft, Exp);
   end Put;

   procedure Put
     (To   : out Wide_String;
      Item : Num;
      Aft  : Field := Default_Aft;
      Exp  : Field := Default_Exp)
   is
      S : String (To'First .. To'Last);

   begin
      if Need64 then
         Aux64.Puts (S, Int64'Integer_Value (Item), Aft, Exp, Scale);
      else
         Aux32.Puts (S, Int32'Integer_Value (Item), Aft, Exp, Scale);
      end if;

      for J in S'Range loop
         To (J) := Wide_Character'Val (Character'Pos (S (J)));
      end loop;
   end Put;

end Ada.Wide_Text_IO.Decimal_IO;
