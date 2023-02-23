------------------------------------------------------------------------------
--                                                                          --
--                         GNAT RUN-TIME COMPONENTS                         --
--                                                                          --
--                      A D A . D I R E C T O R I E S                       --
--                                                                          --
--                                 B o d y                                  --
--                                                                          --
--          Copyright (C) 2004-2023, Free Software Foundation, Inc.         --
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

with Ada.Calendar;               use Ada.Calendar;
with Ada.Characters.Handling;    use Ada.Characters.Handling;
with Ada.Containers.Vectors;
with Ada.Directories.Validity;   use Ada.Directories.Validity;
with Ada.Directories.Hierarchical_File_Names;
use  Ada.Directories.Hierarchical_File_Names;
with Ada.Strings.Fixed;
with Ada.Strings.Maps;           use Ada.Strings.Maps;
with Ada.Strings.Unbounded;      use Ada.Strings.Unbounded;
with Ada.Unchecked_Deallocation;

with Interfaces.C;

with System;                 use System;
with System.CRTL;            use System.CRTL;
with System.File_Attributes; use System.File_Attributes;
with System.File_IO;         use System.File_IO;
with System.OS_Constants;    use System.OS_Constants;
with System.OS_Lib;          use System.OS_Lib;
with System.Regexp;          use System.Regexp;

package body Ada.Directories is

   type Dir_Type_Value is new Address;
   --  This is the low-level address directory structure as returned by the C
   --  opendir routine.

   No_Dir : constant Dir_Type_Value := Dir_Type_Value (Null_Address);
   --  Null directory value

   Dir_Separator : constant Character;
   pragma Import (C, Dir_Separator, "__gnat_dir_separator");
   --  Running system default directory separator

   Dir_Seps : constant Character_Set := Strings.Maps.To_Set ("/\");
   --  UNIX and DOS style directory separators

   Max_Path : Integer;
   pragma Import (C, Max_Path, "__gnat_max_path_len");
   --  The maximum length of a path

   function C_Modification_Time (N : System.Address) return Ada.Calendar.Time;
   pragma Import (C, C_Modification_Time, "__gnat_file_time");
   --  Get modification time for file with name referenced by N

   Invalid_Time : constant Ada.Calendar.Time :=
                    C_Modification_Time (System.Null_Address);
   --  Result returned from C_Modification_Time call when routine unable to get
   --  file modification time.

   Empty_String : constant String := "";
   --  Empty string, returned by function Extension when there is no extension

   ----------------------------
   -- Directory Search Types --
   ----------------------------

   package Directory_Vectors is new
     Ada.Containers.Vectors
       (Index_Type   => Natural,
        Element_Type => Directory_Entry_Type);
   use Directory_Vectors;
   --  Used to store the results of the directory search

   type Dir_Contents_Ptr is access Directory_Vectors.Vector;

   procedure Free is new Ada.Unchecked_Deallocation
     (Directory_Vectors.Vector, Dir_Contents_Ptr);
   --  Directory_Vectors.Vector deallocation routine

   type Search_State is new Ada.Finalization.Controlled with record
      Dir_Contents : Dir_Contents_Ptr;
      Next_Entry   : Cursor;
   end record;
   --  The Search_State consists of a vector of directory items that match the
   --  search pattern and filter, and a cursor pointing to the next item of the
   --  vector to be returned to the user.

   procedure Free is new Ada.Unchecked_Deallocation (Search_State, Search_Ptr);
   --  Search_State deallocation routine

   Dir_Vector_Initial_Size : constant := 100;
   --  Initial size for the Dir_Contents vector, sized to ensure the vector
   --  does not need to be reallocated for reasonably sized directory searches.

   ------------------------
   -- Helper Subprograms --
   ------------------------

   function File_Exists (Name : String) return Boolean;
   --  Returns True if the named file exists

   procedure Start_Search_Internal
     (Search           : in out Search_Type;
      Directory        : String;
      Pattern          : String;
      Filter           : Filter_Type := [others => True];
      Case_Insensitive : Boolean);
   --  Similar to Start_Search except we can specify a case-insensitive search.
   --  This enables detecting the name-case equivalence for a given directory.

   ---------------
   -- Base_Name --
   ---------------

   function Base_Name (Name : String) return String is
      Simple : constant String := Simple_Name (Name);
      --  Simple'First is guaranteed to be 1

   begin
      --  Look for the last dot in the file name and return the part of the
      --  file name preceding this last dot. If the first dot is the first
      --  character of the file name, the base name is the empty string.

      for Pos in reverse Simple'Range loop
         if Simple (Pos) = '.' then
            return Simple (1 .. Pos - 1);
         end if;
      end loop;

      --  If there is no dot, return the complete file name

      return Simple;
   end Base_Name;

   -------------
   -- Compose --
   -------------

   function Compose
     (Containing_Directory : String := "";
      Name                 : String;
      Extension            : String := "") return String
   is
      Result : String (1 .. Containing_Directory'Length +
                              Name'Length + Extension'Length + 2);
      Last   : Natural;

   begin
      --  First, deal with the invalid cases

      if Containing_Directory /= ""
        and then not Is_Valid_Path_Name (Containing_Directory)
      then
         raise Name_Error with
           "invalid directory path name """ & Containing_Directory & '"';

      elsif
        Extension'Length = 0 and then (not Is_Valid_Simple_Name (Name))
      then
         raise Name_Error with
           "invalid simple name """ & Name & '"';

      elsif Extension'Length /= 0
        and then not Is_Valid_Simple_Name (Name & '.' & Extension)
      then
         raise Name_Error with
           "invalid file name """ & Name & '.' & Extension & '"';

      --  This is not an invalid case so build the path name

      else
         Last := Containing_Directory'Length;
         Result (1 .. Last) := Containing_Directory;

         --  Add a directory separator if needed

         if Last /= 0 and then not Is_In (Result (Last), Dir_Seps) then
            Last := Last + 1;
            Result (Last) := Dir_Separator;
         end if;

         --  Add the file name

         Result (Last + 1 .. Last + Name'Length) := Name;
         Last := Last + Name'Length;

         --  If extension was specified, add dot followed by this extension

         if Extension'Length /= 0 then
            Last := Last + 1;
            Result (Last) := '.';
            Result (Last + 1 .. Last + Extension'Length) := Extension;
            Last := Last + Extension'Length;
         end if;

         return Result (1 .. Last);
      end if;
   end Compose;

   --------------------------
   -- Containing_Directory --
   --------------------------

   function Containing_Directory (Name : String) return String is
   begin
      --  First, the invalid case

      if not Is_Valid_Path_Name (Name) then
         raise Name_Error with "invalid path name """ & Name & '"';

      else
         declare
            Last_DS : constant Natural :=
              Strings.Fixed.Index (Name, Dir_Seps, Going => Strings.Backward);

         begin
            --  If Name indicates a root directory, raise Use_Error, because
            --  it has no containing directory.

            if Is_Parent_Directory_Name (Name)
              or else Is_Current_Directory_Name (Name)
              or else Is_Root_Directory_Name (Name)
            then
               raise Use_Error with
                 "directory """ & Name & """ has no containing directory";

            elsif Last_DS = 0 then
               --  There is no directory separator, so return ".", representing
               --  the current working directory.

               return ".";

            else
               declare
                  Last   : Positive := Last_DS - Name'First + 1;
                  Result : String (1 .. Last);

               begin
                  Result := Name (Name'First .. Last_DS);

                  --  Remove any trailing directory separator, except as the
                  --  first character or the first character following a drive
                  --  number on Windows.

                  while Last > 1 loop
                     exit when Is_Root_Directory_Name (Result (1 .. Last))
                                 or else (Result (Last) /= Directory_Separator
                                           and then Result (Last) /= '/');

                     Last := Last - 1;
                  end loop;

                  return Result (1 .. Last);
               end;
            end if;
         end;
      end if;
   end Containing_Directory;

   ---------------
   -- Copy_File --
   ---------------

   procedure Copy_File
     (Source_Name : String;
      Target_Name : String;
      Form        : String := "")
   is
      Success  : Boolean;
      Mode     : Copy_Mode := Overwrite;
      Preserve : Attribute := None;

   begin
      --  First, the invalid cases

      if not Is_Valid_Path_Name (Source_Name) then
         raise Name_Error with
           "invalid source path name """ & Source_Name & '"';

      elsif not Is_Valid_Path_Name (Target_Name) then
         raise Name_Error with
           "invalid target path name """ & Target_Name & '"';

      elsif not Is_Regular_File (Source_Name) then
         raise Name_Error with '"' & Source_Name & """ is not a file";

      elsif Is_Directory (Target_Name) then
         raise Use_Error with "target """ & Target_Name & """ is a directory";

      else
         if Form'Length > 0 then
            declare
               Formstr : String (1 .. Form'Length + 1);
               V1, V2  : Natural;

            begin
               --  Acquire form string, setting required NUL terminator

               Formstr (1 .. Form'Length) := Form;
               Formstr (Formstr'Last) := ASCII.NUL;

               --  Convert form string to lower case

               for J in Formstr'Range loop
                  if Formstr (J) in 'A' .. 'Z' then
                     Formstr (J) :=
                       Character'Val (Character'Pos (Formstr (J)) + 32);
                  end if;
               end loop;

               --  Check Form

               Form_Parameter (Formstr, "mode", V1, V2);

               if V1 = 0 then
                  Mode := Overwrite;
               elsif Formstr (V1 .. V2) = "copy" then
                  Mode := Copy;
               elsif Formstr (V1 .. V2) = "overwrite" then
                  Mode := Overwrite;
               elsif Formstr (V1 .. V2) = "append" then
                  Mode := Append;
               else
                  raise Use_Error with "invalid Form";
               end if;

               Form_Parameter (Formstr, "preserve", V1, V2);

               if V1 = 0 then
                  Preserve := None;
               elsif Formstr (V1 .. V2) = "timestamps" then
                  Preserve := Time_Stamps;
               elsif Formstr (V1 .. V2) = "all_attributes" then
                  Preserve := Full;
               elsif Formstr (V1 .. V2) = "no_attributes" then
                  Preserve := None;
               else
                  raise Use_Error with "invalid Form";
               end if;
            end;
         end if;

         --  Do actual copy using System.OS_Lib.Copy_File

         Copy_File (Source_Name, Target_Name, Success, Mode, Preserve);

         if not Success then
            raise Use_Error with "copy of """ & Source_Name & """ failed";
         end if;
      end if;
   end Copy_File;

   ----------------------
   -- Create_Directory --
   ----------------------

   procedure Create_Directory
     (New_Directory : String;
      Form          : String := "")
   is
      Dir_Name_C : constant String := New_Directory & ASCII.NUL;

   begin
      --  First, the invalid case

      if not Is_Valid_Path_Name (New_Directory) then
         raise Name_Error with
           "invalid new directory path name """ & New_Directory & '"';

      else
         --  Acquire setting of encoding parameter

         declare
            Formstr : constant String := To_Lower (Form);

            Encoding : CRTL.Filename_Encoding;
            --  Filename encoding specified into the form parameter

            V1, V2 : Natural;

         begin
            Form_Parameter (Formstr, "encoding", V1, V2);

            if V1 = 0 then
               Encoding := CRTL.Unspecified;
            elsif Formstr (V1 .. V2) = "utf8" then
               Encoding := CRTL.UTF8;
            elsif Formstr (V1 .. V2) = "8bits" then
               Encoding := CRTL.ASCII_8bits;
            else
               raise Use_Error with "invalid Form";
            end if;

            if CRTL.mkdir (Dir_Name_C, Encoding) /= 0 then
               raise Use_Error with
                 "creation of new directory """ & New_Directory & """ failed";
            end if;
         end;
      end if;
   end Create_Directory;

   -----------------
   -- Create_Path --
   -----------------

   procedure Create_Path
     (New_Directory : String;
      Form          : String := "")
   is
      New_Dir : String (1 .. New_Directory'Length + 1);
      Last    : Positive := 1;
      Start   : Positive := 1;

   begin
      --  First, the invalid case

      if not Is_Valid_Path_Name (New_Directory) then
         raise Name_Error with
           "invalid new directory path name """ & New_Directory & '"';

      else
         --  Build New_Dir with a directory separator at the end, so that the
         --  complete path will be found in the loop below.

         New_Dir (1 .. New_Directory'Length) := New_Directory;
         New_Dir (New_Dir'Last) := Directory_Separator;

         --  If host is windows, and the first two characters are directory
         --  separators, we have an UNC path. Skip it.

         if Directory_Separator = '\'
           and then New_Dir'Length > 2
           and then Is_In (New_Dir (1), Dir_Seps)
           and then Is_In (New_Dir (2), Dir_Seps)
         then
            Start := 2;
            loop
               Start := Start + 1;
               exit when Start = New_Dir'Last
                 or else Is_In (New_Dir (Start), Dir_Seps);
            end loop;
         end if;

         --  Create, if necessary, each directory in the path

         for J in Start + 1 .. New_Dir'Last loop

            --  Look for the end of an intermediate directory

            if not Is_In (New_Dir (J), Dir_Seps) then
               Last := J;

            --  We have found a new intermediate directory each time we find
            --  a first directory separator.

            elsif not Is_In (New_Dir (J - 1), Dir_Seps) then

               --  No need to create the directory if it already exists

               if not Is_Directory (New_Dir (1 .. Last)) then
                  begin
                     Create_Directory
                       (New_Directory => New_Dir (1 .. Last), Form => Form);

                  exception
                     when Use_Error =>
                        if File_Exists (New_Dir (1 .. Last)) then

                           --  A file with such a name already exists. If it is
                           --  a directory, then it was apparently just created
                           --  by another process or thread, and all is well.
                           --  If it is of some other kind, report an error.

                           if not Is_Directory (New_Dir (1 .. Last)) then
                              raise Use_Error with
                                "file """ & New_Dir (1 .. Last) &
                                  """ already exists and is not a directory";
                           end if;

                        else
                           --  Create_Directory failed for some other reason:
                           --  propagate the exception.

                           raise;
                        end if;
                  end;
               end if;
            end if;
         end loop;
      end if;
   end Create_Path;

   -----------------------
   -- Current_Directory --
   -----------------------

   function Current_Directory return String is
      Path_Len : Natural := Max_Path;
      Buffer   : String (1 .. 1 + Max_Path + 1);

      procedure Local_Get_Current_Dir (Dir : Address; Length : Address);
      pragma Import (C, Local_Get_Current_Dir, "__gnat_get_current_dir");

   begin
      Local_Get_Current_Dir (Buffer'Address, Path_Len'Address);

      if Path_Len = 0 then
         raise Use_Error with "current directory does not exist";
      end if;

      --  We need to resolve links because of RM A.16(47), which requires
      --  that we not return alternative names for files.

      return Normalize_Pathname (Buffer (1 .. Path_Len));
   end Current_Directory;

   ----------------------
   -- Delete_Directory --
   ----------------------

   procedure Delete_Directory (Directory : String) is
   begin
      --  First, the invalid cases

      if not Is_Valid_Path_Name (Directory) then
         raise Name_Error with
           "invalid directory path name """ & Directory & '"';

      elsif not Is_Directory (Directory) then
         raise Name_Error with '"' & Directory & """ not a directory";

      --  Do the deletion, checking for error

      else
         declare
            Dir_Name_C : constant String := Directory & ASCII.NUL;
         begin
            if rmdir (Dir_Name_C) /= 0 then
               raise Use_Error with
                 "deletion of directory """ & Directory & """ failed";
            end if;
         end;
      end if;
   end Delete_Directory;

   -----------------
   -- Delete_File --
   -----------------

   procedure Delete_File (Name : String) is
      Success : Boolean;

   begin
      --  First, the invalid cases

      if not Is_Valid_Path_Name (Name) then
         raise Name_Error with "invalid path name """ & Name & '"';

      elsif not Is_Regular_File (Name)
        and then not Is_Symbolic_Link (Name)
      then
         raise Name_Error with "file """ & Name & """ does not exist";

      else
         --  Do actual deletion using System.OS_Lib.Delete_File

         Delete_File (Name, Success);

         if not Success then
            raise Use_Error with "file """ & Name & """ could not be deleted";
         end if;
      end if;
   end Delete_File;

   -----------------
   -- Delete_Tree --
   -----------------

   procedure Delete_Tree (Directory : String) is
      Search      : Search_Type;
      Dir_Ent     : Directory_Entry_Type;
   begin
      --  First, the invalid cases

      if not Is_Valid_Path_Name (Directory) then
         raise Name_Error with
           "invalid directory path name """ & Directory & '"';

      elsif not Is_Directory (Directory) then
         raise Name_Error with '"' & Directory & """ not a directory";

      else

         --  We used to change the current directory to Directory here,
         --  allowing the use of a local Simple_Name for all references. This
         --  turned out unfriendly to multitasking programs, where tasks
         --  running in parallel of this Delete_Tree could see their current
         --  directory change unpredictably. We now resort to Full_Name
         --  computations to reach files and subdirs instead.

         Start_Search (Search, Directory => Directory, Pattern => "");
         while More_Entries (Search) loop
            Get_Next_Entry (Search, Dir_Ent);

            declare
               Fname : constant String := Full_Name   (Dir_Ent);
               Sname : constant String := Simple_Name (Dir_Ent);

            begin
               if OS_Lib.Is_Directory (Fname) then
                  if Sname /= "." and then Sname /= ".." then
                     Delete_Tree (Fname);
                  end if;
               else
                  Delete_File (Fname);
               end if;
            end;
         end loop;

         End_Search (Search);

         declare
            Dir_Name_C : constant String := Directory & ASCII.NUL;

         begin
            if rmdir (Dir_Name_C) /= 0 then
               raise Use_Error with
                 "directory tree rooted at """ &
                   Directory & """ could not be deleted";
            end if;
         end;
      end if;
   end Delete_Tree;

   ------------
   -- Exists --
   ------------

   function Exists (Name : String) return Boolean is
   begin
      --  First, the invalid case

      if not Is_Valid_Path_Name (Name) then
         raise Name_Error with "invalid path name """ & Name & '"';

      else
         --  The implementation is in File_Exists

         return File_Exists (Name);
      end if;
   end Exists;

   ---------------
   -- Extension --
   ---------------

   function Extension (Name : String) return String is
   begin
      --  First, the invalid case

      if not Is_Valid_Path_Name (Name) then
         raise Name_Error with "invalid path name """ & Name & '"';

      else
         --  Look for first dot that is not followed by a directory separator

         for Pos in reverse Name'Range loop

            --  If a directory separator is found before a dot, there is no
            --  extension.

            if Is_In (Name (Pos), Dir_Seps) then
               return Empty_String;

            elsif Name (Pos) = '.' then

               --  We found a dot, build the return value with lower bound 1

               declare
                  subtype Result_Type is String (1 .. Name'Last - Pos);
               begin
                  return Result_Type (Name (Pos + 1 .. Name'Last));
               end;
            end if;
         end loop;

         --  No dot were found, there is no extension

         return Empty_String;
      end if;
   end Extension;

   -----------------
   -- File_Exists --
   -----------------

   function File_Exists (Name : String) return Boolean is
      function C_File_Exists (A : Address) return Integer;
      pragma Import (C, C_File_Exists, "__gnat_file_exists");

      C_Name : String (1 .. Name'Length + 1);

   begin
      C_Name (1 .. Name'Length) := Name;
      C_Name (C_Name'Last) := ASCII.NUL;
      return C_File_Exists (C_Name'Address) = 1;
   end File_Exists;

   --------------
   -- Finalize --
   --------------

   procedure Finalize (Search : in out Search_Type) is
   begin
      if Search.State /= null then
         Free (Search.State.Dir_Contents);
         Free (Search.State);
      end if;
   end Finalize;

   ---------------
   -- Full_Name --
   ---------------

   function Full_Name (Name : String) return String is
   begin
      --  First, the invalid case

      if not Is_Valid_Path_Name (Name) then
         raise Name_Error with "invalid path name """ & Name & '"';

      else
         --  Build the return value with lower bound 1

         --  Use System.OS_Lib.Normalize_Pathname

         declare
            --  We need to resolve links because of (RM A.16(47)), which says
            --  we must not return alternative names for files.

            Value : constant String := Normalize_Pathname (Name);
            subtype Result is String (1 .. Value'Length);

         begin
            return Result (Value);
         end;
      end if;
   end Full_Name;

   function Full_Name (Directory_Entry : Directory_Entry_Type) return String is
   begin
      --  If the Directory_Entry is valid return the full name contained in the
      --  entry record.

      if not Directory_Entry.Valid then
         raise Status_Error with "invalid directory entry";
      else
         return To_String (Directory_Entry.Full_Name);
      end if;
   end Full_Name;

   --------------------
   -- Get_Next_Entry --
   --------------------

   procedure Get_Next_Entry
     (Search          : in out Search_Type;
      Directory_Entry : out Directory_Entry_Type)
   is
   begin
      --  A Search with no state implies the user has not called Start_Search

      if Search.State = null then
         raise Status_Error with "search not started";
      end if;

      --  If the next entry is No_Element it means the search is finished and
      --  there are no more entries to return.

      if Search.State.Next_Entry = No_Element then
         raise Status_Error with "no more entries";
      end if;

      --  Populate Directory_Entry with the next entry and update the search
      --  state.

      Directory_Entry := Element (Search.State.Next_Entry);
      Next (Search.State.Next_Entry);

      --  If Start_Search received a non-zero error code when trying to read
      --  the file attributes of this entry, raise an Use_Error so the user
      --  is aware that it was not possible to retrieve the attributes of this
      --  entry.

      if Directory_Entry.Attr_Error_Code /= 0 then
         raise Use_Error
           with To_String (Directory_Entry.Full_Name) & ": " &
             Errno_Message (Err => Directory_Entry.Attr_Error_Code);
      end if;
   end Get_Next_Entry;

   ----------
   -- Kind --
   ----------

   function Kind (Name : String) return File_Kind is
   begin
      --  First, the invalid case

      if not File_Exists (Name) then
         raise Name_Error with "file """ & Name & """ does not exist";

      --  If OK, return appropriate kind

      elsif Is_Regular_File (Name) then
         return Ordinary_File;

      elsif Is_Directory (Name) then
         return Directory;

      else
         return Special_File;
      end if;
   end Kind;

   function Kind (Directory_Entry : Directory_Entry_Type) return File_Kind is
   begin
      if not Directory_Entry.Valid then
         raise Status_Error with "invalid directory entry";
      else
         return Directory_Entry.Kind;
      end if;
   end Kind;

   -----------------------
   -- Modification_Time --
   -----------------------

   function Modification_Time (Name : String) return Time is

      Date   : Time;
      C_Name : aliased String (1 .. Name'Length + 1);
   begin
      --  First, the invalid cases

      if not (Is_Regular_File (Name) or else Is_Directory (Name)) then
         raise Name_Error with '"' & Name & """ not a file or directory";

      else
         C_Name := Name & ASCII.NUL;
         Date := C_Modification_Time (C_Name'Address);

         if Date = Invalid_Time then
            raise Use_Error with
              "Unable to get modification time of the file """ & Name & '"';
         end if;

         return Date;
      end if;
   end Modification_Time;

   function Modification_Time
     (Directory_Entry : Directory_Entry_Type) return Ada.Calendar.Time
   is
   begin
      --  If the Directory_Entry is valid return the modification time
      --  contained in the entry record. The modification time is recorded in
      --  the entry since its cheap to query all the file the attributes in
      --  one read when the directory is searched.

      if not Directory_Entry.Valid then
         raise Status_Error with "invalid directory entry";
      else
         return Directory_Entry.Modification_Time;
      end if;
   end Modification_Time;

   ------------------
   -- More_Entries --
   ------------------

   function More_Entries (Search : Search_Type) return Boolean is
   begin
      --  If the vector cursor Search.State.Next_Entry points to an element in
      --  Search.State.Dir_Contents then there is another entry to return.
      --  Otherwise, we return False.

      if Search.State = null then
         return False;
      elsif Search.State.Next_Entry = No_Element then
         return False;
      else
         return True;
      end if;
   end More_Entries;

   ---------------------------
   -- Name_Case_Equivalence --
   ---------------------------

   function Name_Case_Equivalence (Name : String) return Name_Case_Kind is
      Dir_Path  : Unbounded_String := To_Unbounded_String (Name);
      S         : Search_Type;
      Test_File : Directory_Entry_Type;

      function GNAT_name_case_equivalence return Interfaces.C.int;
      pragma Import (C, GNAT_name_case_equivalence,
                     "__gnat_name_case_equivalence");

   begin
      --  Check for the invalid case

      if not Is_Valid_Path_Name (Name) then
         raise Name_Error with "invalid path name """ & Name & '"';
      end if;

      --  We were passed a "full path" to a file and not a directory, so obtain
      --  the containing directory.

      if Is_Regular_File (Name) then
         Dir_Path := To_Unbounded_String (Containing_Directory (Name));
      end if;

      --  Since we must obtain a file within the Name directory, let's grab the
      --  first for our test. When the directory is empty, Get_Next_Entry will
      --  fall through to a Status_Error where we then take the imprecise
      --  default for the host OS.

      Start_Search
        (Search    => S,
         Directory => To_String (Dir_Path),
         Pattern   => "",
         Filter    => [Directory => False, others => True]);

      loop
         Get_Next_Entry (S, Test_File);

         --  Check if we have found a "caseable" file

         exit when To_Lower (Simple_Name (Test_File)) /=
                   To_Upper (Simple_Name (Test_File));
      end loop;

      End_Search (S);

      --  Search for files within the directory with the same name, but
      --  differing cases.

      Start_Search_Internal
        (Search                 => S,
         Directory              => To_String (Dir_Path),
         Pattern                => Simple_Name (Test_File),
         Filter                 => [Directory => False, others => True],
         Case_Insensitive => True);

      --  We will find at least one match due to the search hitting our test
      --  file.

      Get_Next_Entry (S, Test_File);

      begin
         --  If we hit two then we know we have a case-sensitive directory

         Get_Next_Entry (S, Test_File);
         End_Search (S);

         return Case_Sensitive;
      exception
         when Status_Error =>
            null;
      end;

      --  Finally, we have a file in the directory whose name is unique and
      --  "caseable". Let's test to see if the OS is able to identify the file
      --  in multiple cases, which will give us our result without having to
      --  resort to defaults.

      if Exists (To_String (Dir_Path) & Directory_Separator
                  & To_Lower (Simple_Name (Test_File)))
        and then Exists (To_String (Dir_Path) & Directory_Separator
                          & To_Upper (Simple_Name (Test_File)))
      then
         return Case_Preserving;
      end if;

      return Case_Sensitive;
   exception
      when Status_Error =>

         --  There is no unobtrusive way to check for the directory's casing so
         --  return the OS default.

         return Name_Case_Kind'Val (Integer (GNAT_name_case_equivalence));
   end Name_Case_Equivalence;

   ------------
   -- Rename --
   ------------

   procedure Rename (Old_Name, New_Name : String) is
      Success : Boolean;

   begin
      --  First, the invalid cases

      if not Is_Valid_Path_Name (Old_Name) then
         raise Name_Error with "invalid old path name """ & Old_Name & '"';

      elsif not Is_Valid_Path_Name (New_Name) then
         raise Name_Error with "invalid new path name """ & New_Name & '"';

      elsif not Is_Regular_File (Old_Name)
            and then not Is_Directory (Old_Name)
      then
         raise Name_Error with "old file """ & Old_Name & """ does not exist";

      elsif Is_Regular_File (New_Name) or else Is_Directory (New_Name) then
         raise Use_Error with
           "new name """ & New_Name
           & """ designates a file that already exists";

      --  Do actual rename using System.OS_Lib.Rename_File

      else
         Rename_File (Old_Name, New_Name, Success);

         if not Success then

            --  AI05-0231-1: Name_Error should be raised in case a directory
            --  component of New_Name does not exist (as in New_Name =>
            --  "/no-such-dir/new-filename"). ENOENT indicates that. ENOENT
            --  also indicate that the Old_Name does not exist, but we already
            --  checked for that above. All other errors are Use_Error.

            if Errno = ENOENT then
               raise Name_Error with
                 "file """ & Containing_Directory (New_Name) & """ not found";

            else
               raise Use_Error with
                 "file """ & Old_Name & """ could not be renamed";
            end if;
         end if;
      end if;
   end Rename;

   ------------
   -- Search --
   ------------

   procedure Search
     (Directory : String;
      Pattern   : String;
      Filter    : Filter_Type := [others => True];
      Process   : not null access procedure
                                    (Directory_Entry : Directory_Entry_Type))
   is
      Srch            : Search_Type;
      Directory_Entry : Directory_Entry_Type;

   begin
      Start_Search (Srch, Directory, Pattern, Filter);
      while More_Entries (Srch) loop
         Get_Next_Entry (Srch, Directory_Entry);
         Process (Directory_Entry);
      end loop;

      End_Search (Srch);
   end Search;

   -------------------
   -- Set_Directory --
   -------------------

   procedure Set_Directory (Directory : String) is
      Dir_Name_C : constant String := Directory & ASCII.NUL;
   begin
      if not Is_Valid_Path_Name (Directory) then
         raise Name_Error with
           "invalid directory path name & """ & Directory & '"';

      elsif not Is_Directory (Directory) then
         raise Name_Error with
           "directory """ & Directory & """ does not exist";

      elsif chdir (Dir_Name_C) /= 0 then
         raise Name_Error with
           "could not set to designated directory """ & Directory & '"';
      end if;
   end Set_Directory;

   -----------------
   -- Simple_Name --
   -----------------

   function Simple_Name (Name : String) return String is

      function Simple_Name_Internal (Path : String) return String;
      --  This function does the job

      --------------------------
      -- Simple_Name_Internal --
      --------------------------

      function Simple_Name_Internal (Path : String) return String is
         Cut_Start : Natural :=
           Strings.Fixed.Index (Path, Dir_Seps, Going => Strings.Backward);

         --  Cut_End points to the last simple name character

         Cut_End   : Natural := Path'Last;

      begin
         --  Root directories are considered simple

         if Is_Root_Directory_Name (Path) then
            return Path;
         end if;

         --  Handle trailing directory separators

         if Cut_Start = Path'Last then
            Cut_End   := Path'Last - 1;
            Cut_Start := Strings.Fixed.Index
                           (Path (Path'First .. Path'Last - 1),
                             Dir_Seps, Going => Strings.Backward);
         end if;

         --  Cut_Start points to the first simple name character

         Cut_Start := (if Cut_Start = 0 then Path'First else Cut_Start + 1);

         Check_For_Standard_Dirs : declare
            BN : constant String := Path (Cut_Start .. Cut_End);

            Has_Drive_Letter : constant Boolean :=
              OS_Lib.Path_Separator /= ':';
            --  If Path separator is not ':' then we are on a DOS based OS
            --  where this character is used as a drive letter separator.

         begin
            if BN = "." or else BN = ".." then
               return BN;

            elsif Has_Drive_Letter
              and then BN'Length > 2
              and then Characters.Handling.Is_Letter (BN (BN'First))
              and then BN (BN'First + 1) = ':'
            then
               --  We have a DOS drive letter prefix, remove it

               return BN (BN'First + 2 .. BN'Last);

            else
               return BN;
            end if;
         end Check_For_Standard_Dirs;
      end Simple_Name_Internal;

   --  Start of processing for Simple_Name

   begin
      --  First, the invalid case

      if not Is_Valid_Path_Name (Name) then
         raise Name_Error with "invalid path name """ & Name & '"';

      else
         --  Build the value to return with lower bound 1

         declare
            Value : constant String := Simple_Name_Internal (Name);
            subtype Result is String (1 .. Value'Length);
         begin
            return Result (Value);
         end;
      end if;
   end Simple_Name;

   function Simple_Name
     (Directory_Entry : Directory_Entry_Type) return String is
   begin
      --  If the Directory_Entry is valid return the simple name contained in
      --  the entry record.

      if not Directory_Entry.Valid then
         raise Status_Error with "invalid directory entry";
      else
         return To_String (Directory_Entry.Name);
      end if;
   end Simple_Name;

   ----------
   -- Size --
   ----------

   function Size (Name : String) return File_Size is
      C_Name : String (1 .. Name'Length + 1);

      function C_Size (Name : Address) return int64;
      pragma Import (C, C_Size, "__gnat_named_file_length");

   begin
      --  First, the invalid case

      if not Is_Regular_File (Name) then
         raise Name_Error with "file """ & Name & """ does not exist";

      else
         C_Name (1 .. Name'Length) := Name;
         C_Name (C_Name'Last) := ASCII.NUL;
         return File_Size (C_Size (C_Name'Address));
      end if;
   end Size;

   function Size (Directory_Entry : Directory_Entry_Type) return File_Size is
   begin
      --  If the Directory_Entry is valid return the size contained in the
      --  entry record. The size is recorded in the entry since it is cheap to
      --  query all the file the attributes in one read when the directory is
      --  searched.

      if not Directory_Entry.Valid then
         raise Status_Error with "invalid directory entry";
      else
         return Directory_Entry.Size;
      end if;
   end Size;

   ------------------
   -- Start_Search --
   ------------------

   procedure Start_Search
     (Search    : in out Search_Type;
      Directory : String;
      Pattern   : String;
      Filter    : Filter_Type := [others => True])
   is
   begin
      Start_Search_Internal (Search, Directory, Pattern, Filter, False);
   end Start_Search;

   ---------------------------
   -- Start_Search_Internal --
   ---------------------------

   procedure Start_Search_Internal
     (Search           : in out Search_Type;
      Directory        : String;
      Pattern          : String;
      Filter           : Filter_Type := [others => True];
      Case_Insensitive : Boolean)
   is
      function closedir (Directory : DIRs) return Integer
        with Import, External_Name => "__gnat_closedir", Convention => C;
      --  C lib function to close Directory

      function opendir (Directory : String) return DIRs
        with Import, External_Name => "__gnat_opendir", Convention => C;
      --  C lib function to open Directory

      function readdir_gnat
        (Directory : Address;
         Buffer    : Address;
         Last      : not null access Integer) return Address
        with Import, External_Name => "__gnat_readdir", Convention => C;
      --  Read the next item in Directory

      Dir_Name_C       : constant String := Directory & ASCII.NUL;
      Dir_Entry_Buffer : array (1 .. SIZEOF_struct_dirent_alloc) of Character;
      Dir_Pointer      : Dir_Type_Value;
      File_Name_Addr   : Address;
      File_Name_Len    : aliased Integer;
      Pattern_Regex    : Regexp;

      Call_Result      : Integer;
      pragma Warnings (Off, Call_Result);
      --  Result of calling a C function that returns a status

   begin
      --  Check that Directory is a valid directory

      if not Is_Directory (Directory) then
         raise Name_Error with
           "unknown directory """ & Simple_Name (Directory) & '"';
      end if;

      --  Check and compile the pattern

      declare
         Case_Sensitive : Boolean := Is_Path_Name_Case_Sensitive;
      begin
         if Case_Insensitive then
            Case_Sensitive := False;
         end if;

         Pattern_Regex :=
           Compile (Pattern, Glob => True, Case_Sensitive => Case_Sensitive);
      exception
         when Error_In_Regexp =>
            raise Name_Error with "invalid pattern """ & Pattern & '"';
      end;

      --  Open Directory

      Dir_Pointer := Dir_Type_Value (opendir (Dir_Name_C));

      if Dir_Pointer = No_Dir then
         raise Use_Error with
           "unreadable directory """ & Simple_Name (Directory) & '"';
      end if;

      --  If needed, finalize Search. Note: we should probably raise an
      --  exception here if Search belongs to an existing search rather than
      --  quietly end it. However, we first need to check that it won't break
      --  existing software.

      Finalize (Search);

      --  Allocate and initialize the search state

      Search.State := new Search_State'
        (Ada.Finalization.Controlled with
         Dir_Contents  => new Vector,
         Next_Entry    => No_Element);

      --  Increase the size of the Dir_Contents vector so it does not need to
      --  grow for most reasonable directory searches.

      Search.State.Dir_Contents.Reserve_Capacity (Dir_Vector_Initial_Size);

      --  Read the contents of Directory into Search.State

      loop
         --  Get next item in the directory

         File_Name_Addr :=
           readdir_gnat
             (Address (Dir_Pointer),
              Dir_Entry_Buffer'Address,
              File_Name_Len'Access);

         exit when File_Name_Addr = Null_Address;

         --  If the file name matches the Pattern and the file type matches
         --  the Filter add it to our search vector.

         declare
            subtype File_Name_String is String (1 .. File_Name_Len);

            File_Name : constant File_Name_String
              with Import, Address => File_Name_Addr;

         begin
            if Match (File_Name, Pattern_Regex) then
               declare
                  Path_C : constant String :=
                             Compose (Directory, File_Name) & ASCII.NUL;
                  Path   : String renames
                             Path_C (Path_C'First .. Path_C'Last - 1);
                  Found  : Boolean := False;
                  Attr   : aliased File_Attributes;
                  Exists : Integer;
                  Error  : Integer;
                  Kind   : File_Kind;
                  Size   : File_Size;

               begin
                  --  Get the file attributes for the directory item

                  Reset_Attributes (Attr'Access);
                  Exists := File_Exists_Attr (Path_C'Address, Attr'Access);
                  Error  := Error_Attributes (Attr'Access);

                  --  If there was an error when trying to read the attributes
                  --  of a Directory entry, record the error so it can be
                  --  propagated to the user when they interate through the
                  --  directory results.

                  if Error /= 0 then
                     Search.State.Dir_Contents.Append
                       (Directory_Entry_Type'
                          (Valid           => True,
                           Name            => To_Unbounded_String (File_Name),
                           Full_Name       => To_Unbounded_String (Path),
                           Attr_Error_Code => Error,
                           others          => <>));

                  --  Otherwise, if the file exists and matches the file kind
                  --  Filter, add the file to the search results. We capture
                  --  the size and modification time here as we have already
                  --  the entry's attributes above.

                  elsif Exists = 1 then
                     if Is_Regular_File_Attr (Path_C'Address, Attr'Access) = 1
                       and then Filter (Ordinary_File)
                     then
                        Found := True;
                        Kind := Ordinary_File;
                        Size :=
                          File_Size
                            (File_Length_Attr
                               (-1, Path_C'Address, Attr'Access));

                     elsif Is_Directory_Attr (Path_C'Address, Attr'Access) = 1
                       and then Filter (File_Kind'First)
                     then
                        Found := True;
                        Kind := File_Kind'First;
                        --  File_Kind'First is used instead of Directory due
                        --  to a name overload issue with the procedure
                        --  parameter Directory.
                        Size := 0;

                     elsif Filter (Special_File) then
                        Found := True;
                        Kind := Special_File;
                        Size := 0;
                     end if;

                     if Found then
                        Search.State.Dir_Contents.Append
                          (Directory_Entry_Type'
                             (Valid             => True,
                              Name              =>
                                To_Unbounded_String (File_Name),
                              Full_Name         => To_Unbounded_String (Path),
                              Attr_Error_Code   => 0,
                              Kind              => Kind,
                              Modification_Time => Modification_Time (Path),
                              Size              => Size));
                     end if;
                  end if;
               end;
            end if;
         end;
      end loop;

      --  Set the first entry to be returned to the user to be the first
      --  element of the Dir_Contents vector. If no items were found, First
      --  will return No_Element, which signals
      Search.State.Next_Entry := Search.State.Dir_Contents.First;

      --  Search is finished, close Directory

      Call_Result := closedir (DIRs (Dir_Pointer));

   end Start_Search_Internal;

end Ada.Directories;
