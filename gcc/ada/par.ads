------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                                  P A R                                   --
--                                                                          --
--                                 S p e c                                  --
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

--  The Par function and its subunits contains all the parsing routines
--  for the top down recursive descent parser that constructs the parse tree

with Types; use Types;

function Par (Configuration_Pragmas : Boolean) return List_Id;
--  Top level parsing routine. There are two cases:
--
--  If Configuration_Pragmas is False, Par parses a compilation unit in the
--  current source file and sets the Cunit, Cunit_Entity and Unit_Name fields
--  of the units table entry for Current_Source_Unit. On return the parse tree
--  is complete, and decorated with any required implicit label declarations.
--  The value returned in this case is always No_List.
--
--  If Configuration_Pragmas is True, Par parses a list of configuration
--  pragmas from the current source file, and returns the list of pragmas.
