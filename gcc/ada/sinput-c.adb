------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                             S I N P U T . C                              --
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

with Debug;  use Debug;
with Opt;    use Opt;
with Output; use Output;
with System; use System;

pragma Warnings (Off);
--  This package is used also by gnatcoll
with System.OS_Lib; use System.OS_Lib;
pragma Warnings (On);

package body Sinput.C is

   ---------------
   -- Load_File --
   ---------------

   function Load_File (Path : String) return Source_File_Index is
      Src  : Source_Buffer_Ptr;
      X    : Source_File_Index;
      Lo   : Source_Ptr;
      Hi   : Source_Ptr;

      Source_File_FD : File_Descriptor;
      --  The file descriptor for the current source file. A negative value
      --  indicates failure to open the specified source file.

      Len : Integer;
      --  Length of file (assume no more than 2 gigabytes of source)

      Actual_Len : Integer;

      Path_Id : File_Name_Type;
      File_Id : File_Name_Type;

   begin
      if Path = "" then
         return No_Source_File;
      end if;

      Source_File.Increment_Last;
      X := Source_File.Last;

      if Debug_Flag_L then
         Write_Str ("Sinput.C.Load_File: created source ");
         Write_Int (Int (X));
         Write_Str (" for ");
         Write_Str (Path);
         Write_Line ("");
      end if;

      if X = Source_File.First then
         Lo := First_Source_Ptr;
      else
         Lo := ((Source_File.Table (X - 1).Source_Last + Source_Align) /
                  Source_Align) * Source_Align;
      end if;

      Name_Len := Path'Length;
      Name_Buffer (1 .. Name_Len) := Path;
      Path_Id := Name_Find;
      Name_Buffer (Name_Len + 1) := ASCII.NUL;

      --  Open the source FD, note that we open in binary mode, because as
      --  documented in the spec, the caller is expected to handle either
      --  DOS or Unix mode files, and there is no point in wasting time on
      --  text translation when it is not required.

      Source_File_FD := Open_Read (Name_Buffer'Address, Binary);

      if Source_File_FD = Invalid_FD then
         Source_File.Decrement_Last;
         return No_Source_File;

      end if;

      Len := Integer (File_Length (Source_File_FD));

      --  Set Hi so that length is one more than the physical length, allowing
      --  for the extra EOF character at the end of the buffer

      Hi := Lo + Source_Ptr (Len);

      --  Do the actual read operation

      declare
         Var_Ptr : constant Source_Buffer_Ptr_Var :=
           new Source_Buffer (Lo .. Hi);
         --  Allocate source buffer, allowing extra character at end for EOF

      begin
         --  Some systems have file types that require one read per line,
         --  so read until we get the Len bytes or until there are no more
         --  characters.

         Hi := Lo;
         loop
            Actual_Len := Read (Source_File_FD, Var_Ptr (Hi)'Address, Len);
            Hi := Hi + Source_Ptr (Actual_Len);
            exit when Actual_Len = Len or else Actual_Len <= 0;
         end loop;

         Var_Ptr (Hi) := EOF;
         Src := Var_Ptr.all'Access;
      end;

      --  Read is complete, close the file and we are done (no need to test
      --  status from close, since we have successfully read the file).

      Close (Source_File_FD);

      --  Get the file name, without path information

      declare
         Index : Positive := Path'Last;

      begin
         while Index > Path'First loop
            exit when Path (Index - 1) = '/';
            exit when Path (Index - 1) = Directory_Separator;
            Index := Index - 1;
         end loop;

         Name_Len := Path'Last - Index + 1;
         Name_Buffer (1 .. Name_Len) := Path (Index .. Path'Last);
         File_Id := Name_Find;
      end;

      declare
         S : Source_File_Record renames Source_File.Table (X);

      begin
         S := (Debug_Source_Name   => File_Id,
               File_Name           => File_Id,
               File_Type           => Config,
               First_Mapped_Line   => No_Line_Number,
               Full_Debug_Name     => Path_Id,
               Full_File_Name      => Path_Id,
               Full_Ref_Name       => Path_Id,
               Instance            => No_Instance_Id,
               Identifier_Casing   => Unknown,
               Inlined_Call        => No_Location,
               Inlined_Body        => False,
               Inherited_Pragma    => False,
               Keyword_Casing      => Unknown,
               Last_Source_Line    => 1,
               License             => Unknown,
               Lines_Table         => null,
               Lines_Table_Max     => 1,
               Logical_Lines_Table => null,
               Num_SRef_Pragmas    => 0,
               Reference_Name      => File_Id,
               Sloc_Adjust         => 0,
               Source_Checksum     => 0,
               Source_First        => Lo,
               Source_Last         => Hi,
               Source_Text         => Src,
               Template            => No_Source_File,
               Unit                => No_Unit,
               Time_Stamp          => Empty_Time_Stamp,
               Index               => X);

         Alloc_Line_Tables (S, Opt.Table_Factor * Alloc.Lines_Initial);
         S.Lines_Table (1) := Lo;
      end;

      Set_Source_File_Index_Table (X);
      return X;
   end Load_File;

end Sinput.C;
