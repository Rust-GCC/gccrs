------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                                E R R I D                                 --
--                                                                          --
--                                 B o d y                                  --
--                                                                          --
--          Copyright (C) 1992-2026, Free Software Foundation, Inc.         --
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

with Errid.Diagnostic_Repository;  use Errid.Diagnostic_Repository;
with Errid.Restriction_Repository; use Errid.Restriction_Repository;
with Errid.Switch_Repository;      use Errid.Switch_Repository;
with Erroutc.SARIF_Emitter;        use Erroutc.SARIF_Emitter;
with Rident;                       use Rident;

package body Errid is

   Doc_Directory : constant String := "./error_codes";
   Doc_Extension : constant String := ".md";

   Diagnostic_Inconsistency : exception;

   procedure Add_All_Diagnostic_Rules (Printer : in out SARIF_Printer);
   --  Add all active Diagnostic_Id-s to the SARIF_Printer

   procedure Add_All_Restriction_Rules (Printer : in out SARIF_Printer);
   --  Add all active Restriction_Id-s to the SARIF_Printer

   procedure Add_All_Switch_Rules (Printer : in out SARIF_Printer);
   --  Add all active Switch_Id-s to the SARIF_Printer

   procedure Check_Diagnostic_To_Switch_Consistency (D_Id : Diagnostic_Id);
   --  Check that if a diagnostic has a switch then that diagnostic is also
   --  included in the list of diagnostics for that switch.

   procedure Check_Diagnostic_To_Restriction_Consistency
     (D_Id : Diagnostic_Id);
   --  Check that if a diagnostic has a restriction then that diagnostic is
   --  also included as the diagnostic for that restriction.

   procedure Check_Restriction_To_Diagnostic_Consistency
     (R_Id : Restriction_Id);
   --  Check that the diagnostic set for a restriction has that restriction as
   --  its restriction in the repository.

   procedure Check_Switch_To_Diagnostic_Consistency (S_Id : Switch_Id);
   --  Check that if a Switch has diagnostics then that diagnostic has the same
   --  switch marked as its switch.

   --------------------------------------------
   -- Check_Diagnostic_To_Switch_Consistency --
   --------------------------------------------

   procedure Check_Diagnostic_To_Switch_Consistency (D_Id : Diagnostic_Id) is
      D       : constant Diagnostic_Entry_Type := Diagnostic_Entries (D_Id);
      Err_Msg : constant String :=
        Switch_Id'Image (D.Switch)
        & " should contain "
        & Diagnostic_Id'Image (D_Id)
        & " in its diagnostics";
   begin
      if D.Switch = No_Switch_Id then
         return;
      end if;

      if Switches (D.Switch).Diagnostics = null then
         raise Diagnostic_Inconsistency with Err_Msg;
      end if;

      for DD of Switches (D.Switch).Diagnostics.all loop
         if D_Id = DD then
            return;
         end if;
      end loop;

      raise Diagnostic_Inconsistency with Err_Msg;
   end Check_Diagnostic_To_Switch_Consistency;

   -------------------------------------------------
   -- Check_Diagnostic_To_Restriction_Consistency --
   -------------------------------------------------

   procedure Check_Diagnostic_To_Restriction_Consistency (D_Id : Diagnostic_Id)
   is
      D        : constant Diagnostic_Entry_Type := Diagnostic_Entries (D_Id);
      Restrict : constant Restriction_Id := D.Restriction;
      Err_Msg  : constant String :=
        Restriction_Id'Image (D.Restriction)
        & " should contain "
        & Diagnostic_Id'Image (D_Id)
        & " in its diagnostics";
   begin
      if Restrict = Not_A_Restriction_Id then
         return;
      end if;

      if Rest_To_Diag_Mappping (Restrict) /= D_Id then
         raise Diagnostic_Inconsistency with Err_Msg;
      end if;
   end Check_Diagnostic_To_Restriction_Consistency;

   -------------------------------------------------
   -- Check_Restriction_To_Diagnostic_Consistency --
   -------------------------------------------------

   procedure Check_Restriction_To_Diagnostic_Consistency
     (R_Id : Restriction_Id)
   is
      D    : Diagnostic_Entry_Type;
      D_Id : Diagnostic_Id;
   begin
      if R_Id = Not_A_Restriction_Id then
         return;
      end if;

      D_Id := Rest_To_Diag_Mappping (R_Id);
      D := Diagnostic_Entries (D_Id);

      if D.Restriction /= R_Id then
         raise Diagnostic_Inconsistency
           with
             Restriction_Id'Image (R_Id)
             & " should be the restriction for "
             & Diagnostic_Id'Image (D_Id);
      end if;
   end Check_Restriction_To_Diagnostic_Consistency;

   --------------------------------------------
   -- Check_Switch_To_Diagnostic_Consistency --
   --------------------------------------------

   procedure Check_Switch_To_Diagnostic_Consistency (S_Id : Switch_Id) is
      S : constant Switch_Type := Switches (S_Id);
      D : Diagnostic_Entry_Type;
   begin
      if S.Diagnostics = null then
         return;
      end if;

      for D_Id of S.Diagnostics.all loop
         D := Diagnostic_Entries (D_Id);
         if D.Switch /= S_Id then
            raise Diagnostic_Inconsistency
              with
                Switch_Id'Image (S_Id)
                & " should be the switch for "
                & Diagnostic_Id'Image (D_Id);
         end if;
      end loop;
   end Check_Switch_To_Diagnostic_Consistency;

   ----------------------------
   -- Get_Documentation_File --
   ----------------------------

   function Get_Documentation_File (Id : Diagnostic_Id) return String is
   begin
      if Id = No_Diagnostic_Id then
         return "";
      else
         return Doc_Directory & "/" & To_String (Id) & Doc_Extension;
      end if;
   end Get_Documentation_File;

   ---------------
   -- To_String --
   ---------------

   function To_String (Id : Diagnostic_Id) return String is
   begin
      if Id = No_Diagnostic_Id then
         return "GNAT0000";
      else
         return Id'Img;
      end if;
   end To_String;

   ------------------------------
   -- Add_All_Diagnostic_Rules --
   ------------------------------

   procedure Add_All_Diagnostic_Rules (Printer : in out SARIF_Printer) is
   begin
      Printer.Diagnostics := Diagnostic_Id_Lists.Create;
      for Id in Diagnostic_Id loop
         if Id /= No_Diagnostic_Id then
            Diagnostic_Id_Lists.Append (Printer.Diagnostics, Id);
            Check_Diagnostic_To_Switch_Consistency (Id);
            Check_Diagnostic_To_Restriction_Consistency (Id);
         end if;
      end loop;
   end Add_All_Diagnostic_Rules;

   -------------------------------
   -- Add_All_Restriction_Rules --
   -------------------------------

   procedure Add_All_Restriction_Rules (Printer : in out SARIF_Printer) is
   begin
      Printer.Restrictions := Restriction_Id_Lists.Create;
      for R in All_Restrictions loop
         Restriction_Id_Lists.Append (Printer.Restrictions, R);
         Check_Restriction_To_Diagnostic_Consistency (R);
      end loop;
   end Add_All_Restriction_Rules;

   --------------------------
   -- Add_All_Switch_Rules --
   --------------------------

   procedure Add_All_Switch_Rules (Printer : in out SARIF_Printer) is
   begin
      Printer.Switches := Switch_Id_Lists.Create;
      for S in Switch_Id loop
         if S /= No_Switch_Id then
            Switch_Id_Lists.Append (Printer.Switches, S);
            Check_Switch_To_Diagnostic_Consistency (S);
         end if;
      end loop;
   end Add_All_Switch_Rules;

   ---------------------------------
   -- Print_Diagnostic_Repository --
   ---------------------------------

   procedure Print_Diagnostic_Repository is
      Printer : SARIF_Printer;
   begin
      Add_All_Diagnostic_Rules (Printer);
      Add_All_Switch_Rules (Printer);
      Add_All_Restriction_Rules (Printer);
      Printer.Report_Type := Repository_Report;

      Print_SARIF_Report (Printer);
      Free (Printer);
   end Print_Diagnostic_Repository;

end Errid;
