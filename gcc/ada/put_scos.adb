------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                             P U T _ S C O S                              --
--                                                                          --
--                                 B o d y                                  --
--                                                                          --
--          Copyright (C) 2009-2023, Free Software Foundation, Inc.         --
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

with Namet;
with Opt;
with SCOs; use SCOs;

procedure Put_SCOs is
   Current_SCO_Unit : SCO_Unit_Index := 0;
   --  Initial value must not be a valid unit index

   procedure Write_SCO_Initiate (SU : SCO_Unit_Index);
   --  Start SCO line for unit SU, also emitting SCO unit header if necessary

   procedure Write_Instance_Table;
   --  Output the SCO table of instances

   procedure Output_Range (T : SCO_Table_Entry);
   --  Outputs T.From and T.To in line:col-line:col format

   procedure Output_Source_Location (Loc : Source_Location);
   --  Output source location in line:col format

   procedure Output_String (S : String);
   --  Output S

   ------------------
   -- Output_Range --
   ------------------

   procedure Output_Range (T : SCO_Table_Entry) is
   begin
      Output_Source_Location (T.From);
      Write_Info_Char ('-');
      Output_Source_Location (T.To);
   end Output_Range;

   ----------------------------
   -- Output_Source_Location --
   ----------------------------

   procedure Output_Source_Location (Loc : Source_Location) is
   begin
      Write_Info_Nat  (Nat (Loc.Line));
      Write_Info_Char (':');
      Write_Info_Nat  (Nat (Loc.Col));
   end Output_Source_Location;

   -------------------
   -- Output_String --
   -------------------

   procedure Output_String (S : String) is
   begin
      for J in S'Range loop
         Write_Info_Char (S (J));
      end loop;
   end Output_String;

   --------------------------
   -- Write_Instance_Table --
   --------------------------

   procedure Write_Instance_Table is
   begin
      for J in 1 .. SCO_Instance_Table.Last loop
         declare
            SIE : SCO_Instance_Table_Entry
                    renames SCO_Instance_Table.Table (J);
         begin
            Output_String ("C i ");
            Write_Info_Nat (Nat (J));
            Write_Info_Char (' ');
            Write_Info_Nat (SIE.Inst_Dep_Num);
            Write_Info_Char ('|');
            Output_Source_Location (SIE.Inst_Loc);

            if SIE.Enclosing_Instance > 0 then
               Write_Info_Char (' ');
               Write_Info_Nat (Nat (SIE.Enclosing_Instance));
            end if;
            Write_Info_Terminate;
         end;
      end loop;
   end Write_Instance_Table;

   ------------------------
   -- Write_SCO_Initiate --
   ------------------------

   procedure Write_SCO_Initiate (SU : SCO_Unit_Index) is
      SUT : SCO_Unit_Table_Entry renames SCO_Unit_Table.Table (SU);

   begin
      if Current_SCO_Unit /= SU then
         Write_Info_Initiate ('C');
         Write_Info_Char (' ');
         Write_Info_Nat (SUT.Dep_Num);
         Write_Info_Char (' ');

         Output_String (SUT.File_Name.all);

         Write_Info_Terminate;

         Current_SCO_Unit := SU;
      end if;

      Write_Info_Initiate ('C');
   end Write_SCO_Initiate;

--  Start of processing for Put_SCOs

begin
   --  Loop through entries in SCO_Unit_Table. Note that entry 0 is by
   --  convention present but unused.

   for U in 1 .. SCO_Unit_Table.Last loop
      declare
         SUT : SCO_Unit_Table_Entry renames SCO_Unit_Table.Table (U);

         Start : Nat;
         Stop  : Nat;

      begin
         Start := SUT.From;
         Stop  := SUT.To;

         --  Loop through SCO entries for this unit

         loop
            exit when Start = Stop + 1;
            pragma Assert (Start <= Stop);

            Output_SCO_Line : declare
               T            : SCO_Table_Entry renames SCO_Table.Table (Start);
               Continuation : Boolean;

               Ctr : Nat;
               --  Counter for statement entries

            begin
               case T.C1 is

                  --  Statements (and dominance markers)

                  when 'S' | '>' =>
                     Ctr := 0;
                     Continuation := False;
                     loop
                        if Ctr = 0 then
                           Write_SCO_Initiate (U);
                           if not Continuation then
                              Write_Info_Char ('S');
                              Continuation := True;
                           else
                              Write_Info_Char ('s');
                           end if;
                        end if;

                        Write_Info_Char (' ');

                        declare
                           Sent : SCO_Table_Entry
                                    renames SCO_Table.Table (Start);
                        begin
                           if Sent.C1 = '>' then
                              Write_Info_Char (Sent.C1);
                           end if;

                           if Sent.C2 /= ' ' then
                              Write_Info_Char (Sent.C2);

                              if Sent.C1 = 'S'
                                and then (Sent.C2 = 'P' or else Sent.C2 = 'p')
                                and then Sent.Pragma_Aspect_Name /= No_Name
                              then
                                 Write_Info_Name (Sent.Pragma_Aspect_Name);
                                 Write_Info_Char (':');
                              end if;
                           end if;

                           --  For dependence markers (except E), output sloc.
                           --  For >E and all statement entries, output sloc
                           --  range.

                           if Sent.C1 = '>' and then Sent.C2 /= 'E' then
                              Output_Source_Location (Sent.From);
                           else
                              Output_Range (Sent);
                           end if;
                        end;

                        --  Increment entry counter (up to 6 entries per line,
                        --  continuation lines are marked Cs).

                        Ctr := Ctr + 1;
                        if Ctr = 6 then
                           Write_Info_Terminate;
                           Ctr := 0;
                        end if;

                        exit when SCO_Table.Table (Start).Last;
                        Start := Start + 1;
                     end loop;

                     if Ctr > 0 then
                        Write_Info_Terminate;
                     end if;

                  --  Decision

                  when 'E' | 'G' | 'I' | 'P' | 'W' | 'X' | 'A' =>
                     Start := Start + 1;

                     Write_SCO_Initiate (U);
                     Write_Info_Char (T.C1);

                     if T.C1 = 'A' then
                        Write_Info_Name (T.Pragma_Aspect_Name);
                     end if;

                     if T.C1 /= 'X' then
                        Write_Info_Char (' ');
                        Output_Source_Location (T.From);
                     end if;

                     --  Loop through table entries for this decision

                     loop
                        declare
                           T : SCO_Table_Entry renames SCO_Table.Table (Start);

                        begin
                           Write_Info_Char (' ');

                           if T.C1 = '!' or else
                              T.C1 = '&' or else
                              T.C1 = '|'
                           then
                              Write_Info_Char (T.C1);
                              pragma Assert (T.C2 /= '?');
                              Output_Source_Location (T.From);

                           else
                              Write_Info_Char (T.C2);
                              Output_Range (T);
                           end if;

                           exit when T.Last;
                           Start := Start + 1;
                        end;
                     end loop;

                     Write_Info_Terminate;

                  when ASCII.NUL =>

                     --  Nullified entry: skip

                     null;

                  when others =>
                     raise Program_Error;
               end case;
            end Output_SCO_Line;

            Start := Start + 1;
         end loop;
      end;
   end loop;

   if Opt.Generate_SCO_Instance_Table then
      Write_Instance_Table;
   end if;
end Put_SCOs;
