------------------------------------------------------------------------------
--                                                                          --
--                         GNAT RUN-TIME COMPONENTS                         --
--                                                                          --
--    A D A . W I D E _ W I D E _ C H A R A C T E R T S . U N I C O D E    --
--                                                                          --
--                                 S p e c                                  --
--                                                                          --
--          Copyright (C) 2005-2023, Free Software Foundation, Inc.         --
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

--  Unicode categorization routines for Wide_Wide_Character

with System.UTF_32;

package Ada.Wide_Wide_Characters.Unicode is
   pragma Pure;

   --  The following type defines the categories from the unicode definitions.
   --  The one addition we make is Fe, which represents the characters FFFE
   --  and FFFF in any of the planes.

   type Category is new System.UTF_32.Category;
   --  Cc   Other, Control
   --  Cf   Other, Format
   --  Cn   Other, Not Assigned
   --  Co   Other, Private Use
   --  Cs   Other, Surrogate
   --  Ll   Letter, Lowercase
   --  Lm   Letter, Modifier
   --  Lo   Letter, Other
   --  Lt   Letter, Titlecase
   --  Lu   Letter, Uppercase
   --  Mc   Mark, Spacing Combining
   --  Me   Mark, Enclosing
   --  Mn   Mark, Nonspacing
   --  Nd   Number, Decimal Digit
   --  Nl   Number, Letter
   --  No   Number, Other
   --  Pc   Punctuation, Connector
   --  Pd   Punctuation, Dash
   --  Pe   Punctuation, Close
   --  Pf   Punctuation, Final quote
   --  Pi   Punctuation, Initial quote
   --  Po   Punctuation, Other
   --  Ps   Punctuation, Open
   --  Sc   Symbol, Currency
   --  Sk   Symbol, Modifier
   --  Sm   Symbol, Math
   --  So   Symbol, Other
   --  Zl   Separator, Line
   --  Zp   Separator, Paragraph
   --  Zs   Separator, Space
   --  Fe   relative position FFFE/FFFF in plane

   function Get_Category (U : Wide_Wide_Character) return Category;
   pragma Inline (Get_Category);
   --  Given a Wide_Wide_Character, returns corresponding Category, or Cn if
   --  the code does not have an assigned unicode category.

   --  The following functions perform category tests corresponding to lexical
   --  classes defined in the Ada standard. There are two interfaces for each
   --  function. The second takes a Category (e.g. returned by Get_Category).
   --  The first takes a Wide_Wide_Character. The form taking the
   --  Wide_Wide_Character is typically more efficient than calling
   --  Get_Category, but if several different tests are to be performed on the
   --  same code, it is more efficient to use Get_Category to get the category,
   --  then test the resulting category.

   function Is_Letter (U : Wide_Wide_Character) return Boolean;
   function Is_Letter (C : Category)            return Boolean;
   pragma Inline (Is_Letter);
   --  Returns true iff U is a letter that can be used to start an identifier,
   --  or if C is one of the corresponding categories, which are the following:
   --    Letter, Uppercase (Lu)
   --    Letter, Lowercase (Ll)
   --    Letter, Titlecase (Lt)
   --    Letter, Modifier  (Lm)
   --    Letter, Other     (Lo)
   --    Number, Letter    (Nl)

   function Is_Digit (U : Wide_Wide_Character) return Boolean;
   function Is_Digit (C : Category)            return Boolean;
   pragma Inline (Is_Digit);
   --  Returns true iff U is a digit that can be used to extend an identifer,
   --  or if C is one of the corresponding categories, which are the following:
   --    Number, Decimal_Digit (Nd)

   function Is_Line_Terminator (U : Wide_Wide_Character) return Boolean;
   pragma Inline (Is_Line_Terminator);
   --  Returns true iff U is an allowed line terminator for source programs,
   --  if U is in the category Zp (Separator, Paragaph), or Zs (Separator,
   --  Line), or if U is a conventional line terminator (CR, LF, VT, FF).
   --  There is no category version for this function, since the set of
   --  characters does not correspond to a set of Unicode categories.

   function Is_Mark (U : Wide_Wide_Character) return Boolean;
   function Is_Mark (C : Category)            return Boolean;
   pragma Inline (Is_Mark);
   --  Returns true iff U is a mark character which can be used to extend an
   --  identifier, or if C is one of the corresponding categories, which are
   --  the following:
   --    Mark, Non-Spacing (Mn)
   --    Mark, Spacing Combining (Mc)

   function Is_Other (U : Wide_Wide_Character) return Boolean;
   function Is_Other (C : Category)            return Boolean;
   pragma Inline (Is_Other);
   --  Returns true iff U is an other format character, which means that it
   --  can be used to extend an identifier, but is ignored for the purposes of
   --  matching of identiers, or if C is one of the corresponding categories,
   --  which are the following:
   --    Other, Format (Cf)

   function Is_Punctuation (U : Wide_Wide_Character) return Boolean;
   function Is_Punctuation (C : Category)            return Boolean;
   pragma Inline (Is_Punctuation);
   --  Returns true iff U is a punctuation character that can be used to
   --  separate pices of an identifier, or if C is one of the corresponding
   --  categories, which are the following:
   --    Punctuation, Connector (Pc)

   function Is_Space (U : Wide_Wide_Character) return Boolean;
   function Is_Space (C : Category)            return Boolean;
   pragma Inline (Is_Space);
   --  Returns true iff U is considered a space to be ignored, or if C is one
   --  of the corresponding categories, which are the following:
   --    Separator, Space (Zs)

   function Is_NFKC (U : Wide_Wide_Character) return Boolean;
   pragma Inline (Is_NFKC);
   --  Returns True if the Wide_Wide_Character designated by U could be present
   --  in a string normalized to Normalization Form KC (as defined by Clause
   --  21 of ISO/IEC 10646:2017), otherwise returns False.

   function Is_Non_Graphic (U : Wide_Wide_Character) return Boolean;
   function Is_Non_Graphic (C : Category)            return Boolean;
   pragma Inline (Is_Non_Graphic);
   --  Returns true iff U is considered to be a non-graphic character, or if C
   --  is one of the corresponding categories, which are the following:
   --    Other, Control (Cc)
   --    Other, Private Use (Co)
   --    Other, Surrogate (Cs)
   --    Separator, Line (Zl)
   --    Separator, Paragraph (Zp)
   --    FFFE or FFFF positions in any plane (Fe)
   --
   --  Note that the Ada category format effector is subsumed by the above
   --  list of Unicode categories.
   --
   --  Note that Other, Unassiged (Cn) is quite deliberately not included
   --  in the list of categories above. This means that should any of these
   --  code positions be defined in future with graphic characters they will
   --  be allowed without a need to change implementations or the standard.
   --
   --  Note that Other, Format (Cf) is also quite deliberately not included
   --  in the list of categories above. This means that these characters can
   --  be included in character and string literals.

   function Is_Basic (U : Wide_Wide_Character) return Boolean;
   pragma Inline (Is_Basic);
   --  Returns True if the Wide_Wide_Character designated by Item has no
   --  Decomposition Mapping in the code charts of ISO/IEC 10646:2017,
   --  otherwise returns False.

   function To_Basic (U : Wide_Wide_Character) return Wide_Wide_Character;
   pragma Inline (To_Basic);
   --  Returns the Wide_Wide_Character whose code point is given by the first
   --  value of its Decomposition Mapping in the code charts of
   --  ISO/IEC 10646:2017 if any, returns Item otherwise.

   --  The following function is used to fold to upper case, as required by
   --  the Ada 2005 standard rules for identifier case folding. Two
   --  identifiers are equivalent if they are identical after folding all
   --  letters to upper case using this routine. A fold to lower routine is
   --  also provided.

   function To_Lower_Case
     (U : Wide_Wide_Character) return Wide_Wide_Character;
   pragma Inline (To_Lower_Case);
   --  If U represents an upper case letter, returns the corresponding lower
   --  case letter, otherwise U is returned unchanged. The folding is locale
   --  independent as defined by documents referenced in the note in section
   --  1 of ISO/IEC 10646:2003

   function To_Upper_Case
     (U : Wide_Wide_Character) return Wide_Wide_Character;
   pragma Inline (To_Upper_Case);
   --  If U represents a lower case letter, returns the corresponding upper
   --  case letter, otherwise U is returned unchanged. The folding is locale
   --  independent as defined by documents referenced in the note in section
   --  1 of ISO/IEC 10646:2003

end Ada.Wide_Wide_Characters.Unicode;
