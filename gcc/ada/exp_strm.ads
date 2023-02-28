------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                             E X P _ S T R M                              --
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

--  Routines to build stream subprograms for composite types

with Exp_Tss; use Exp_Tss;
with Types;   use Types;

package Exp_Strm is

   function Build_Elementary_Input_Call (N : Node_Id) return Node_Id;
   --  Build call to Read attribute function for elementary type. Also used
   --  for Input attributes for elementary types with an appropriate extra
   --  assignment statement. N is the attribute reference node.

   function Build_Elementary_Write_Call (N : Node_Id) return Node_Id;
   --  Build call to Write attribute function for elementary type. Also used
   --  for Output attributes for elementary types (since the effect of the
   --  two attributes is identical for elementary types). N is the attribute
   --  reference node.

   function Build_Stream_Attr_Profile
     (Loc : Source_Ptr;
      Typ : Entity_Id;
      Nam : TSS_Name_Type) return List_Id;
   --  Builds the parameter profile for the stream attribute identified by
   --  the given name. This is used for the tagged case to build the spec
   --  for the primitive operation.

   --  The following routines build procedures and functions for stream
   --  attributes applied to composite types. For each of these routines,
   --  Loc is used to provide the location for the constructed subprogram
   --  declaration. Typ is the base type to which the subprogram applies
   --  (i.e. the base type of the stream attribute prefix). The returned
   --  results are the declaration and name (entity) of the subprogram.

   procedure Build_Array_Input_Function
     (Loc  : Source_Ptr;
      Typ  : Entity_Id;
      Decl : out Node_Id;
      Fnam : out Entity_Id);
   --  Build function for Input attribute for array type

   procedure Build_Array_Output_Procedure
     (Loc  : Source_Ptr;
      Typ  : Entity_Id;
      Decl : out Node_Id;
      Pnam : out Entity_Id);
   --  Build procedure for Output attribute for array type

   procedure Build_Array_Read_Procedure
     (Nod  : Node_Id;
      Typ  : Entity_Id;
      Decl : out Node_Id;
      Pnam : out Entity_Id);
   --  Build procedure for Read attribute for array type. Nod provides the
   --  Sloc value for generated code.

   procedure Build_Array_Write_Procedure
     (Nod  : Node_Id;
      Typ  : Entity_Id;
      Decl : out Node_Id;
      Pnam : out Entity_Id);
   --  Build procedure for Write attribute for array type. Nod provides the
   --  Sloc value for generated code.

   procedure Build_Mutable_Record_Read_Procedure
     (Loc  : Source_Ptr;
      Typ  : Entity_Id;
      Decl : out Node_Id;
      Pnam : out Entity_Id);
   --  Build procedure to Read a record with default discriminants.
   --  Discriminants must be read explicitly (RM 13.13.2(9)) in the
   --  same manner as is done for 'Input.

   procedure Build_Mutable_Record_Write_Procedure
     (Loc  : Source_Ptr;
      Typ  : Entity_Id;
      Decl : out Node_Id;
      Pnam : out Entity_Id);
   --  Build procedure to write a record with default discriminants.
   --  Discriminants must be written explicitly (RM 13.13.2(9)) in
   --  the same manner as is done for 'Output.

   procedure Build_Record_Or_Elementary_Input_Function
     (Loc            : Source_Ptr;
      Typ            : Entity_Id;
      Decl           : out Node_Id;
      Fnam           : out Entity_Id);
   --  Build function for Input attribute for record type or for an elementary
   --  type (the latter is used only in the case where a user-defined Read
   --  routine is defined, since, in other cases, Input calls the appropriate
   --  runtime library routine directly).

   procedure Build_Record_Or_Elementary_Output_Procedure
     (Loc  : Source_Ptr;
      Typ  : Entity_Id;
      Decl : out Node_Id;
      Pnam : out Entity_Id);
   --  Build procedure for Output attribute for record type or for an
   --  elementary type (the latter is used only in the case where a
   --  user defined Write routine is defined, since in other cases,
   --  Output calls the appropriate runtime library routine directly.

   procedure Build_Record_Read_Procedure
     (Loc  : Source_Ptr;
      Typ  : Entity_Id;
      Decl : out Node_Id;
      Pnam : out Entity_Id);
   --  Build procedure for Read attribute for record type

   procedure Build_Record_Write_Procedure
     (Loc  : Source_Ptr;
      Typ  : Entity_Id;
      Decl : out Node_Id;
      Pnam : out Entity_Id);
   --  Build procedure for Write attribute for record type

   procedure Build_Stream_Procedure
     (Loc  : Source_Ptr;
      Typ  : Entity_Id;
      Decl : out Node_Id;
      Pnam : Entity_Id;
      Stms : List_Id;
      Outp : Boolean);
   --  Called to build an array or record stream procedure. The first three
   --  arguments are the same as Build_Record_Or_Elementary_Output_Procedure.
   --  Stms is the list of statements for the body (the declaration list is
   --  always null), and Pnam is the name of the constructed procedure.
   --  Used by Exp_Dist to generate stream-oriented attributes for RACWs.

end Exp_Strm;
