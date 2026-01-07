------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--           E R R I D . D I A G N O S T I C _ R E P O S I T O R Y          --
--                                                                          --
--                                 S p e c                                  --
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

with Rident; use Rident;
with Types;  use Types;

package Errid.Diagnostic_Repository is
   type Diagnostic_Entry_Type is record
      Status : Status_Type := Active;

      Human_Id : String_Ptr := null;
      --  A human readable code for the diagnostic. If the diagnostic has a
      --  switch with a human id then the human_id of the switch shall be used
      --  in SARIF reports.

      Switch : Switch_Id := No_Switch_Id;
      --  The switch that controls the diagnostic message.

      Restriction : Restriction_Id := Not_A_Restriction_Id;
      --  The restriction associated with the diagnostic message.
   end record;

   type Diagnostics_Registry_Type is
     array (Diagnostic_Id) of Diagnostic_Entry_Type;

   --  Include the diagnostic entries for every diagnostic id.
   --  The entries should include:
   --  * Whether the diagnostic with this id is active or not
   --  * The human-readable name for the diagnostic for SARIF reports
   --  * The switch id for the diagnostic if the diagnostic is linked to any
   --    compiler switch
   --  * The documentation file for the diagnostic written in the MD format.
   --    The documentation file should include:
   --    - The diagnostic id
   --    - A short description of the diagnostic
   --    - A minimal example of the code that triggers the diagnostic
   --    - An explanation of why the diagnostic was triggered
   --    - A suggestion on how to fix the issue
   --    - Optionally additional information
   --    TODO: the mandatory fields for the documentation file could be changed

   Diagnostic_Entries : constant Diagnostics_Registry_Type :=
     (No_Diagnostic_Id => <>,
      GNAT0001         =>
        (Status      => Active,
         Human_Id    => new String'("Default_Iterator_Not_Primitive_Error"),
         Switch      => No_Switch_Id,
         Restriction => Not_A_Restriction_Id),
      GNAT0002         =>
        (Status      => Active,
         Human_Id    => new String'("Invalid_Operand_Types_General_Error"),
         Switch      => No_Switch_Id,
         Restriction => Not_A_Restriction_Id),
      GNAT0003         =>
        (Status      => Active,
         Human_Id    => new String'("Pragma_No_Effect_With_Lock_Free_Warning"),
         Switch      => No_Switch_Id,
         Restriction => Not_A_Restriction_Id),
      GNAT0004         =>
        (Status      => Active,
         Human_Id    => new String'("End_Loop_Expected_Error"),
         Switch      => No_Switch_Id,
         Restriction => Not_A_Restriction_Id),
      GNAT0005         =>
        (Status      => Active,
         Human_Id    => new String'("Representation_Too_Late_Error"),
         Switch      => No_Switch_Id,
         Restriction => Not_A_Restriction_Id),
      GNAT0006         =>
        (Status      => Active,
         Human_Id    => new String'("Mixed_Container_Aggregate_Error"),
         Switch      => No_Switch_Id,
         Restriction => Not_A_Restriction_Id),
      GNAT0007         =>
        (Status      => Active,
         Human_Id    => new String'("Volatile_Not_Modifed_Warning"),
         Switch      => gnatwk,
         Restriction => Not_A_Restriction_Id),
      GNAT0008         =>
        (Status      => Active,
         Human_Id    => new String'("Variable_Not_Modifed_Warning"),
         Switch      => gnatwk,
         Restriction => Not_A_Restriction_Id),
      GNAT0009         =>
        (Status      => Active,
         Human_Id    => new String'("In_Out_Not_Modifed_Warning"),
         Switch      => gnatwk,
         Restriction => Not_A_Restriction_Id),
      GNAT0010         =>
        (Status      => Active,
         Human_Id    => new String'("Simple_Barriers_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => Simple_Barriers),
      GNAT0011         =>
        (Status      => Active,
         Human_Id    => new String'("Pure_Barriers_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => Pure_Barriers),
      GNAT0012         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Abort_Statements_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Abort_Statements),
      GNAT0013         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Access_Parameter_Allocators_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Access_Parameter_Allocators),
      GNAT0014         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Access_Subprograms_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Access_Subprograms),
      GNAT0015         =>
        (Status      => Active,
         Human_Id    => new String'("No_Allocators_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Allocators),
      GNAT0016         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Anonymous_Allocators_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Anonymous_Allocators),
      GNAT0017         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Asynchronous_Control_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Asynchronous_Control),
      GNAT0018         =>
        (Status      => Active,
         Human_Id    => new String'("No_Calendar_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Calendar),
      GNAT0019         =>
        (Status      => Active,
         Human_Id    => new String'("No_Coextensions_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Coextensions),
      GNAT0020         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Default_Stream_Attributes_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Default_Stream_Attributes),
      GNAT0021         =>
        (Status      => Active,
         Human_Id    => new String'("No_Delay_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Delay),
      GNAT0022         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Direct_Boolean_Operators_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Direct_Boolean_Operators),
      GNAT0023         =>
        (Status      => Active,
         Human_Id    => new String'("No_Dispatch_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Dispatch),
      GNAT0024         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Dispatching_Calls_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Dispatching_Calls),
      GNAT0025         =>
        (Status      => Active,
         Human_Id    =>
           new String'
             ("No_Dynamic_Accessibility_Checks_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Dynamic_Accessibility_Checks),
      GNAT0026         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Dynamic_Attachment_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Dynamic_Attachment),
      GNAT0027         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Dynamic_CPU_Assignment_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Dynamic_CPU_Assignment),
      GNAT0028         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Dynamic_Priorities_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Dynamic_Priorities),
      GNAT0029         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Enumeration_Maps_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Enumeration_Maps),
      GNAT0030         =>
        (Status      => Active,
         Human_Id    =>
           new String'
             ("No_Entry_Calls_In_Elaboration_Code_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Entry_Calls_In_Elaboration_Code),
      GNAT0031         =>
        (Status      => Active,
         Human_Id    => new String'("No_Entry_Queue_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Entry_Queue),
      GNAT0032         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Exception_Handlers_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Exception_Handlers),
      GNAT0033         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Exception_Propagation_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Exception_Propagation),
      GNAT0034         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Exception_Registration_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Exception_Registration),
      GNAT0035         =>
        (Status      => Active,
         Human_Id    => new String'("No_Exceptions_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Exceptions),
      GNAT0036         =>
        (Status      => Active,
         Human_Id    => new String'("No_Finalization_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Finalization),
      GNAT0037         =>
        (Status      => Active,
         Human_Id    => new String'("No_Fixed_IO_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Fixed_IO),
      GNAT0038         =>
        (Status      => Active,
         Human_Id    => new String'("No_Fixed_Point_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Fixed_Point),
      GNAT0039         =>
        (Status      => Active,
         Human_Id    => new String'("No_Floating_Point_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Floating_Point),
      GNAT0040         =>
        (Status      => Active,
         Human_Id    => new String'("No_IO_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_IO),
      GNAT0041         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Implicit_Conditionals_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Implicit_Conditionals),
      GNAT0042         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Implicit_Dynamic_Code_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Implicit_Dynamic_Code),
      GNAT0043         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Implicit_Heap_Allocations_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Implicit_Heap_Allocations),
      GNAT0044         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Implicit_Task_Allocations_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Implicit_Task_Allocations),
      GNAT0045         =>
        (Status      => Active,
         Human_Id    =>
           new String'
             ("No_Implicit_Protected_Object_Allocations"
              & "_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Implicit_Protected_Object_Allocations),
      GNAT0046         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Initialize_Scalars_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Initialize_Scalars),
      GNAT0047         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Local_Tagged_Types_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Local_Tagged_Types),
      GNAT0048         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Local_Allocators_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Local_Allocators),
      GNAT0049         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Local_Timing_Events_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Local_Timing_Events),
      GNAT0050         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Local_Protected_Objects_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Local_Protected_Objects),
      GNAT0051         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Long_Long_Integers_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Long_Long_Integers),
      GNAT0052         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Multiple_Elaboration_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Multiple_Elaboration),
      GNAT0053         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Nested_Finalization_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Nested_Finalization),
      GNAT0054         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Protected_Type_Allocators_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Protected_Type_Allocators),
      GNAT0055         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Protected_Types_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Protected_Types),
      GNAT0056         =>
        (Status      => Active,
         Human_Id    => new String'("No_Recursion_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Recursion),
      GNAT0057         =>
        (Status      => Active,
         Human_Id    => new String'("No_Reentrancy_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Reentrancy),
      GNAT0058         =>
        (Status      => Active,
         Human_Id    => new String'("No_Relative_Delay_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Relative_Delay),
      GNAT0059         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Requeue_Statements_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Requeue_Statements),
      GNAT0060         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Secondary_Stack_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Secondary_Stack),
      GNAT0061         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Select_Statements_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Select_Statements),
      GNAT0062         =>
        (Status      => Active,
         Human_Id    =>
           new String'
             ("No_Specific_Termination_Handlers_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Specific_Termination_Handlers),
      GNAT0063         =>
        (Status      => Active,
         Human_Id    =>
           new String'
             ("No_Standard_Allocators_After_Elaboration"
              & "_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Standard_Allocators_After_Elaboration),
      GNAT0064         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Standard_Storage_Pools_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Standard_Storage_Pools),
      GNAT0065         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Stream_Optimizations_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Stream_Optimizations),
      GNAT0066         =>
        (Status      => Active,
         Human_Id    => new String'("No_Streams_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Streams),
      GNAT0067         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Tagged_Type_Registration_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Tagged_Type_Registration),
      GNAT0068         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Task_Allocators_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Task_Allocators),
      GNAT0069         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Task_Attributes_Package_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Task_Attributes_Package),
      GNAT0070         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Task_At_Interrupt_Priority_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Task_At_Interrupt_Priority),
      GNAT0071         =>
        (Status      => Active,
         Human_Id    => new String'("No_Task_Hierarchy_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Task_Hierarchy),
      GNAT0072         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Task_Hierarchy_Implicit_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Task_Hierarchy_Implicit),
      GNAT0073         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Task_Termination_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Task_Termination),
      GNAT0074         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Tasks_Unassigned_To_CPU_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Tasks_Unassigned_To_CPU),
      GNAT0075         =>
        (Status      => Active,
         Human_Id    => new String'("No_Tasking_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Tasking),
      GNAT0076         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Terminate_Alternatives_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Terminate_Alternatives),
      GNAT0077         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Unchecked_Access_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Unchecked_Access),
      GNAT0078         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Unchecked_Conversion_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Unchecked_Conversion),
      GNAT0079         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Unchecked_Deallocation_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Unchecked_Deallocation),
      GNAT0080         =>
        (Status      => Active,
         Human_Id    => new String'("Static_Priorities_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => Static_Priorities),
      GNAT0081         =>
        (Status      => Active,
         Human_Id    =>
           new String'("Static_Storage_Size_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => Static_Storage_Size),
      GNAT0082         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Default_Initialization_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Default_Initialization),
      GNAT0083         =>
        (Status      => Active,
         Human_Id    =>
           new String'("Immediate_Reclamation_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => Immediate_Reclamation),
      GNAT0084         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Dynamic_Sized_Objects_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Dynamic_Sized_Objects),
      GNAT0085         =>
        (Status      => Active,
         Human_Id    =>
           new String'
             ("No_Implementation_Aspect_Specifications_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Implementation_Aspect_Specifications),
      GNAT0086         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Implementation_Attributes_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Implementation_Attributes),
      GNAT0087         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Implementation_Identifiers_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Implementation_Identifiers),
      GNAT0088         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Implementation_Pragmas_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Implementation_Pragmas),
      GNAT0089         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Implementation_Restrictions_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Implementation_Restrictions),
      GNAT0090         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Implementation_Units_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Implementation_Units),
      GNAT0091         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Implicit_Aliasing_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Implicit_Aliasing),
      GNAT0092         =>
        (Status      => Active,
         Human_Id    => new String'("No_Implicit_Loops_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Implicit_Loops),
      GNAT0093         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Elaboration_Code_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Elaboration_Code),
      GNAT0094         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Obsolescent_Features_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Obsolescent_Features),
      GNAT0095         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Unrecognized_Aspects_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Unrecognized_Aspects),
      GNAT0096         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Unrecognized_Pragmas_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Unrecognized_Pragmas),
      GNAT0097         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Wide_Characters_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Wide_Characters),
      GNAT0098         =>
        (Status      => Active,
         Human_Id    =>
           new String'("Static_Dispatch_Tables_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => Static_Dispatch_Tables),
      GNAT0099         =>
        (Status      => Active,
         Human_Id    => new String'("SPARK_05_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => SPARK_05),
      GNAT0100         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Specification_Of_Aspect_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Specification_Of_Aspect),
      GNAT0101         =>
        (Status      => Active,
         Human_Id    =>
           new String'("No_Use_Of_Attribute_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Use_Of_Attribute),
      GNAT0102         =>
        (Status      => Active,
         Human_Id    => new String'("No_Use_Of_Pragma_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => No_Use_Of_Pragma),
      GNAT0103         =>
        (Status      => Active,
         Human_Id    =>
           new String'("Max_Protected_Entries_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => Max_Protected_Entries),
      GNAT0104         =>
        (Status      => Active,
         Human_Id    =>
           new String'("Max_Select_Alternatives_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => Max_Select_Alternatives),
      GNAT0105         =>
        (Status      => Active,
         Human_Id    => new String'("Max_Task_Entries_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => Max_Task_Entries),
      GNAT0106         =>
        (Status      => Active,
         Human_Id    => new String'("Max_Tasks_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => Max_Tasks),
      GNAT0107         =>
        (Status      => Active,
         Human_Id    =>
           new String'
             ("Max_Asynchronous_Select_Nesting_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => Max_Asynchronous_Select_Nesting),
      GNAT0108         =>
        (Status      => Active,
         Human_Id    =>
           new String'("Max_Entry_Queue_Length_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => Max_Entry_Queue_Length),
      GNAT0109         =>
        (Status      => Active,
         Human_Id    =>
           new String'("Max_Storage_At_Blocking_Restriction_Violation"),
         Switch      => No_Switch_Id,
         Restriction => Max_Storage_At_Blocking));
end Errid.Diagnostic_Repository;
