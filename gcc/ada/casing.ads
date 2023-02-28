------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                               C A S I N G                                --
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

with Namet; use Namet;
with Types; use Types;

package Casing is

   --  This package contains data and subprograms to support the feature that
   --  recognizes the letter case styles used in the source program being
   --  compiled, and uses this information for error message formatting, and
   --  for recognizing reserved words that are misused as identifiers.

   -------------------------------
   -- Case Control Declarations --
   -------------------------------

   --  Declaration of type for describing casing convention

   type Casing_Type is (

      All_Upper_Case,
      --  All letters are upper case

      All_Lower_Case,
      --  All letters are lower case

      Mixed_Case,
      --  The initial letter, and any letters after underlines are upper case.
      --  All other letters are lower case

      Unknown
      --  Used if an identifier does not distinguish between the above cases,
      --  (e.g. X, Y_3, M4, A_B, or if it is inconsistent ABC_def).
   );

   subtype Known_Casing is Casing_Type range All_Upper_Case .. Mixed_Case;
   --  Exclude Unknown casing

   ------------------------------
   -- Case Control Subprograms --
   ------------------------------

   procedure Set_Casing
     (Buf : in out Bounded_String;
      C   : Casing_Type;
      D   : Casing_Type := Mixed_Case);
   --  Takes the name stored in Buf and modifies it to be consistent with the
   --  casing given by C, or if C = Unknown, then with the casing given by
   --  D. The name is basically treated as an identifier, except that special
   --  separator characters other than underline are permitted and treated like
   --  underlines (this handles cases like minus and period in unit names,
   --  apostrophes in error messages, angle brackets in names like <any_type>,
   --  etc).

   procedure Set_Casing (C : Casing_Type; D : Casing_Type := Mixed_Case);
   --  Uses Buf => Global_Name_Buffer

   function Determine_Casing (Ident : Text_Buffer) return Casing_Type;
   --  Determines the casing of the identifier/keyword string Ident. A special
   --  test is made for SPARK_Mode which is considered to be mixed case, since
   --  this gives a better general behavior.

end Casing;
