------------------------------------------------------------------------------
--                                                                          --
--                         GNAT RUN-TIME COMPONENTS                         --
--                                                                          --
--           A D A . W I D E _ T E X T _ IO . C O M P L E X _ I O           --
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

with Ada.Wide_Text_IO.Complex_Aux;
with Ada.Wide_Text_IO.Float_Aux;
with System.Img_Flt;  use System.Img_Flt;
with System.Img_LFlt; use System.Img_LFlt;
with System.Img_LLF;  use System.Img_LLF;
with System.Val_Flt;  use System.Val_Flt;
with System.Val_LFlt; use System.Val_LFlt;
with System.Val_LLF;  use System.Val_LLF;
with System.WCh_Con;  use System.WCh_Con;
with System.WCh_WtS;  use System.WCh_WtS;

package body Ada.Wide_Text_IO.Complex_IO is

   use Complex_Types;

   package Scalar_Float is new
     Ada.Wide_Text_IO.Float_Aux (Float, Scan_Float, Set_Image_Float);

   package Scalar_Long_Float is new
     Ada.Wide_Text_IO.Float_Aux
       (Long_Float, Scan_Long_Float, Set_Image_Long_Float);

   package Scalar_Long_Long_Float is new
     Ada.Wide_Text_IO.Float_Aux
       (Long_Long_Float, Scan_Long_Long_Float, Set_Image_Long_Long_Float);

   package Aux_Float is new
     Ada.Wide_Text_IO.Complex_Aux (Float, Scalar_Float);

   package Aux_Long_Float is new
     Ada.Wide_Text_IO.Complex_Aux (Long_Float, Scalar_Long_Float);

   package Aux_Long_Long_Float is new
     Ada.Wide_Text_IO.Complex_Aux (Long_Long_Float, Scalar_Long_Long_Float);

   --  Throughout this generic body, we distinguish between the case where type
   --  Float is OK, where type Long_Float is OK and where type Long_Long_Float
   --  is needed. These boolean constants are used to test for this, such that
   --  only code for the relevant case is included in the instance.

   OK_Float : constant Boolean := Real'Base'Digits <= Float'Digits;

   OK_Long_Float : constant Boolean := Real'Base'Digits <= Long_Float'Digits;

   ---------
   -- Get --
   ---------

   procedure Get
     (File  : File_Type;
      Item  : out Complex;
      Width : Field := 0)
   is
      Real_Item : Real'Base;
      Imag_Item : Real'Base;

   begin
      if OK_Float then
         Aux_Float.Get (File, Float (Real_Item), Float (Imag_Item), Width);
      elsif OK_Long_Float then
         Aux_Long_Float.Get
           (File, Long_Float (Real_Item), Long_Float (Imag_Item), Width);
      else
         Aux_Long_Long_Float.Get
           (File, Long_Long_Float (Real_Item), Long_Long_Float (Imag_Item),
            Width);
      end if;

      Item := (Real_Item, Imag_Item);

   exception
      when Constraint_Error => raise Data_Error;
   end Get;

   ---------
   -- Get --
   ---------

   procedure Get
     (Item  : out Complex;
      Width : Field := 0)
   is
   begin
      Get (Current_In, Item, Width);
   end Get;

   ---------
   -- Get --
   ---------

   procedure Get
     (From : Wide_String;
      Item : out Complex;
      Last : out Positive)
   is
      Real_Item : Real'Base;
      Imag_Item : Real'Base;

      S : constant String := Wide_String_To_String (From, WCEM_Upper);
      --  String on which we do the actual conversion. Note that the method
      --  used for wide character encoding is irrelevant, since if there is
      --  a character outside the Standard.Character range then the call to
      --  Aux.Gets will raise Data_Error in any case.

   begin
      if OK_Float then
         Aux_Float.Gets (S, Float (Real_Item), Float (Imag_Item), Last);
      elsif OK_Long_Float then
         Aux_Long_Float.Gets
           (S, Long_Float (Real_Item), Long_Float (Imag_Item), Last);
      else
         Aux_Long_Long_Float.Gets
           (S, Long_Long_Float (Real_Item), Long_Long_Float (Imag_Item),
            Last);
      end if;

      Item := (Real_Item, Imag_Item);

   exception
      when Data_Error => raise Constraint_Error;
   end Get;

   ---------
   -- Put --
   ---------

   procedure Put
     (File : File_Type;
      Item : Complex;
      Fore : Field := Default_Fore;
      Aft  : Field := Default_Aft;
      Exp  : Field := Default_Exp)
   is
   begin
      if OK_Float then
         Aux_Float.Put
           (File, Float (Re (Item)), Float (Im (Item)), Fore, Aft, Exp);
      elsif OK_Long_Float then
         Aux_Long_Float.Put
           (File, Long_Float (Re (Item)), Long_Float (Im (Item)), Fore, Aft,
            Exp);
      else
         Aux_Long_Long_Float.Put
           (File, Long_Long_Float (Re (Item)), Long_Long_Float (Im (Item)),
            Fore, Aft, Exp);
      end if;
   end Put;

   ---------
   -- Put --
   ---------

   procedure Put
     (Item : Complex;
      Fore : Field := Default_Fore;
      Aft  : Field := Default_Aft;
      Exp  : Field := Default_Exp)
   is
   begin
      Put (Current_Out, Item, Fore, Aft, Exp);
   end Put;

   ---------
   -- Put --
   ---------

   procedure Put
     (To   : out Wide_String;
      Item : Complex;
      Aft  : Field := Default_Aft;
      Exp  : Field := Default_Exp)
   is
      S : String (To'First .. To'Last);

   begin
      if OK_Float then
         Aux_Float.Puts (S, Float (Re (Item)), Float (Im (Item)), Aft, Exp);
      elsif OK_Long_Float then
         Aux_Long_Float.Puts
           (S, Long_Float (Re (Item)), Long_Float (Im (Item)), Aft, Exp);
      else
         Aux_Long_Long_Float.Puts
           (S, Long_Long_Float (Re (Item)), Long_Long_Float (Im (Item)),
            Aft, Exp);
      end if;

      for J in S'Range loop
         To (J) := Wide_Character'Val (Character'Pos (S (J)));
      end loop;
   end Put;

end Ada.Wide_Text_IO.Complex_IO;
