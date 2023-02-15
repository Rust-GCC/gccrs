------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                             S I N F O . C N                              --
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

--  This child package of Sinfo contains some routines that permit in place
--  alteration of existing tree nodes by changing the value in the Nkind
--  field. Since Nkind functions logically in a manner similar to a variant
--  record discriminant part, such alterations cannot be permitted in a
--  general manner, but in some specific cases, the fields of related nodes
--  have been deliberately laid out in a manner that permits such alteration.

package Sinfo.CN is

   procedure Change_Identifier_To_Defining_Identifier (N : Node_Id);
   --  N must refer to a node of type N_Identifier. This node is modified to
   --  be of type N_Defining_Identifier. The scanner always returns identifiers
   --  as N_Identifier. The parser then uses this routine to change the node
   --  to be a defining identifier where the context demands it. This routine
   --  also allocates the necessary extension node.

   procedure Change_Character_Literal_To_Defining_Character_Literal
     (N : Node_Id);
   --  Similar processing for a character literal

   procedure Change_Operator_Symbol_To_Defining_Operator_Symbol
     (N : Node_Id);
   --  Similar processing for an operator symbol

   procedure Change_Conversion_To_Unchecked (N : Node_Id);
   --  Change checked conversion node to unchecked conversion node, clearing
   --  irrelevant check flags (other fields in the two nodes are identical)

   procedure Change_Operator_Symbol_To_String_Literal (N : Node_Id);
   --  The scanner returns any string that looks like an operator symbol as
   --  a N_Operator_Symbol node. The parser then uses this procedure to change
   --  the node to a normal N_String_Literal node if the context is not one
   --  in which an operator symbol is required. There are some cases where the
   --  parser cannot tell, in which case this transformation happens later on.

   procedure Change_Selected_Component_To_Expanded_Name (N : Node_Id);
   --  The parser always generates Selected_Component nodes. The semantics
   --  modifies these to Expanded_Name nodes where appropriate. Note that
   --  on return the Chars field is set to a copy of the contents of the
   --  Chars field of the Selector_Name field.

   procedure Change_Name_To_Procedure_Call_Statement (N : Node_Id);
   --  Some statements (procedure call statements) are in the form of a name
   --  and are parsed as such. This routine takes the scanned name as input
   --  and returns the corresponding N_Procedure_Call_Statement.

end Sinfo.CN;
