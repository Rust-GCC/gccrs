------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--  G N A T . D I R E C T O R Y _ O P E R A T I O N S . I T E R A T I O N   --
--                                                                          --
--                                 B o d y                                  --
--                                                                          --
--                     Copyright (C) 2001-2023, AdaCore                     --
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

with Ada.Characters.Handling;
with Ada.Strings.Fixed;
with Ada.Strings.Maps;

with GNAT.OS_Lib;
with GNAT.Regexp;

package body GNAT.Directory_Operations.Iteration is

   use Ada;

   ----------
   -- Find --
   ----------

   procedure Find
     (Root_Directory : Dir_Name_Str;
      File_Pattern   : String)
   is
      File_Regexp : constant Regexp.Regexp := Regexp.Compile (File_Pattern);
      Index       : Natural := 0;
      Quit        : Boolean := False;

      procedure Read_Directory (Directory : Dir_Name_Str);
      --  Open Directory and read all entries. This routine is called
      --  recursively for each sub-directories.

      function Make_Pathname (Dir, File : String) return String;
      --  Returns the pathname for File by adding Dir as prefix

      -------------------
      -- Make_Pathname --
      -------------------

      function Make_Pathname (Dir, File : String) return String is
      begin
         if Dir (Dir'Last) = '/' or else Dir (Dir'Last) = '\' then
            return Dir & File;
         else
            return Dir & Dir_Separator & File;
         end if;
      end Make_Pathname;

      --------------------
      -- Read_Directory --
      --------------------

      procedure Read_Directory (Directory : Dir_Name_Str) is
         Buffer : String (1 .. 2_048);
         Last   : Natural;

         Dir : Dir_Type;
         pragma Warnings (Off, Dir);

      begin
         Open (Dir, Directory);

         loop
            Read (Dir, Buffer, Last);
            exit when Last = 0;

            declare
               Dir_Entry : constant String := Buffer (1 .. Last);
               Pathname  : constant String :=
                             Make_Pathname (Directory, Dir_Entry);

            begin
               if Regexp.Match (Dir_Entry, File_Regexp) then
                  Index := Index + 1;

                  begin
                     Action (Pathname, Index, Quit);
                  exception
                     when others =>
                        Close (Dir);
                        raise;
                  end;

                  exit when Quit;
               end if;

               --  Recursively call for sub-directories, except for . and ..

               if not (Dir_Entry = "." or else Dir_Entry = "..")
                 and then OS_Lib.Is_Directory (Pathname)
                 and then not OS_Lib.Is_Symbolic_Link (Pathname)
               then
                  Read_Directory (Pathname);
                  exit when Quit;
               end if;
            end;
         end loop;

         Close (Dir);
      end Read_Directory;

   begin
      Read_Directory (Root_Directory);
   end Find;

   -----------------------
   -- Wildcard_Iterator --
   -----------------------

   procedure Wildcard_Iterator (Path : Path_Name) is

      Index : Natural := 0;

      procedure Read
        (Directory      : String;
         File_Pattern   : String;
         Suffix_Pattern : String);
      --  Read entries in Directory and call user's callback if the entry match
      --  File_Pattern and Suffix_Pattern is empty; otherwise go down one more
      --  directory level by calling Next_Level routine below.

      procedure Next_Level
        (Current_Path : String;
         Suffix_Path  : String);
      --  Extract next File_Pattern from Suffix_Path and call Read routine
      --  above.

      ----------------
      -- Next_Level --
      ----------------

      procedure Next_Level
        (Current_Path : String;
         Suffix_Path  : String)
      is
         DS : Natural;
         SP : String renames Suffix_Path;

      begin
         if SP'Length > 2
           and then SP (SP'First) = '.'
           and then Strings.Maps.Is_In (SP (SP'First + 1), Dir_Seps)
         then
            --  Starting with "./"

            DS := Strings.Fixed.Index
              (SP (SP'First + 2 .. SP'Last),
               Dir_Seps);

            if DS = 0 then

               --  We have "./"

               Read (Current_Path & ".", "*", "");

            else
               --  We have "./dir"

               Read (Current_Path & ".",
                     SP (SP'First + 2 .. DS - 1),
                     SP (DS .. SP'Last));
            end if;

         elsif SP'Length > 3
           and then SP (SP'First .. SP'First + 1) = ".."
           and then Strings.Maps.Is_In (SP (SP'First + 2), Dir_Seps)
         then
            --  Starting with "../"

            DS := Strings.Fixed.Index
                    (SP (SP'First + 3 .. SP'Last), Dir_Seps);

            if DS = 0 then

               --  We have "../"

               Read (Current_Path & "..", "*", "");

            else
               --  We have "../dir"

               Read (Current_Path & "..",
                     SP (SP'First + 3 .. DS - 1),
                     SP (DS .. SP'Last));
            end if;

         elsif Current_Path = ""
           and then SP'Length > 1
           and then Characters.Handling.Is_Letter (SP (SP'First))
           and then SP (SP'First + 1) = ':'
         then
            --  Starting with "<drive>:"

            if SP'Length > 2
              and then Strings.Maps.Is_In (SP (SP'First + 2), Dir_Seps)
            then
               --  Starting with "<drive>:\"

               DS := Strings.Fixed.Index
                       (SP (SP'First + 3 .. SP'Last), Dir_Seps);

               if DS = 0 then

                  --  We have "<drive>:\dir"

                  Read (SP (SP'First .. SP'First + 2),
                        SP (SP'First + 3 .. SP'Last),
                        "");

               else
                  --  We have "<drive>:\dir\kkk"

                  Read (SP (SP'First .. SP'First + 2),
                        SP (SP'First + 3 .. DS - 1),
                        SP (DS .. SP'Last));
               end if;

            else
               --  Starting with "<drive>:" and the drive letter not followed
               --  by a directory separator. The proper semantic on Windows is
               --  to read the content of the current selected directory on
               --  this drive. For example, if drive C current selected
               --  directory is c:\temp the suffix pattern "c:m*" is
               --  equivalent to c:\temp\m*.

               DS :=  Strings.Fixed.Index
                        (SP (SP'First + 2 .. SP'Last), Dir_Seps);

               if DS = 0 then

                  --  We have "<drive>:dir"

                  Read (SP, "", "");

               else
                  --  We have "<drive>:dir/kkk"

                  Read (SP (SP'First .. DS - 1), "", SP (DS .. SP'Last));
               end if;
            end if;

         elsif Strings.Maps.Is_In (SP (SP'First), Dir_Seps) then

            --  Starting with a /

            DS := Strings.Fixed.Index
                    (SP (SP'First + 1 .. SP'Last), Dir_Seps);

            if DS = 0 then

               --  We have "/dir"

               Read (Current_Path, SP (SP'First + 1 .. SP'Last), "");
            else
               --  We have "/dir/kkk"

               Read (Current_Path,
                     SP (SP'First + 1 .. DS - 1),
                     SP (DS .. SP'Last));
            end if;

         else
            --  Starting with a name

            DS := Strings.Fixed.Index (SP, Dir_Seps);

            if DS = 0 then

               --  We have "dir"

               Read (Current_Path & '.', SP, "");
            else
               --  We have "dir/kkk"

               Read (Current_Path & '.',
                     SP (SP'First .. DS - 1),
                     SP (DS .. SP'Last));
            end if;

         end if;
      end Next_Level;

      ----------
      -- Read --
      ----------

      Quit : Boolean := False;
      --  Global state to be able to exit all recursive calls

      procedure Read
        (Directory      : String;
         File_Pattern   : String;
         Suffix_Pattern : String)
      is
         File_Regexp : constant Regexp.Regexp :=
                         Regexp.Compile (File_Pattern, Glob => True);

         Dir : Dir_Type;
         pragma Warnings (Off, Dir);

         Buffer : String (1 .. 2_048);
         Last   : Natural;

      begin
         if OS_Lib.Is_Directory (Directory & Dir_Separator) then
            Open (Dir, Directory & Dir_Separator);

            Dir_Iterator : loop
               Read (Dir, Buffer, Last);
               exit Dir_Iterator when Last = 0;

               declare
                  Dir_Entry : constant String := Buffer (1 .. Last);
                  Pathname  : constant String :=
                                Directory & Dir_Separator & Dir_Entry;
               begin
                  --  Handle "." and ".." only if explicit use in the
                  --  File_Pattern.

                  if not
                    ((Dir_Entry = "." and then File_Pattern /= ".")
                       or else
                     (Dir_Entry = ".." and then File_Pattern /= ".."))
                  then
                     if Regexp.Match (Dir_Entry, File_Regexp) then
                        if Suffix_Pattern = "" then

                           --  No more matching needed, call user's callback

                           Index := Index + 1;

                           begin
                              Action (Pathname, Index, Quit);
                           exception
                              when others =>
                                 Close (Dir);
                                 raise;
                           end;

                        else
                           --  Down one level

                           Next_Level
                             (Directory & Dir_Separator & Dir_Entry,
                              Suffix_Pattern);
                        end if;
                     end if;
                  end if;
               end;

               --  Exit if Quit set by call to Action, either at this level
               --  or at some lower recursive call to Next_Level.

               exit Dir_Iterator when Quit;
            end loop Dir_Iterator;

            Close (Dir);
         end if;
      end Read;

   --  Start of processing for Wildcard_Iterator

   begin
      if Path = "" then
         return;
      end if;

      Next_Level ("", Path);
   end Wildcard_Iterator;

end GNAT.Directory_Operations.Iteration;
