------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--          E R R I D . R E S T R I C T I O N _ R E P O S I T O R Y         --
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

package Errid.Restriction_Repository is

   type Restriction_To_Diagnostic_Mapping is
     array (Restriction_Id) of Diagnostic_Id;

   Rest_To_Diag_Mappping : constant Restriction_To_Diagnostic_Mapping :=
     (Simple_Barriers                          => GNAT0010,
      Pure_Barriers                            => GNAT0011,
      No_Abort_Statements                      => GNAT0012,
      No_Access_Parameter_Allocators           => GNAT0013,
      No_Access_Subprograms                    => GNAT0014,
      No_Allocators                            => GNAT0015,
      No_Anonymous_Allocators                  => GNAT0016,
      No_Asynchronous_Control                  => GNAT0017,
      No_Calendar                              => GNAT0018,
      No_Coextensions                          => GNAT0019,
      No_Default_Stream_Attributes             => GNAT0020,
      No_Delay                                 => GNAT0021,
      No_Direct_Boolean_Operators              => GNAT0022,
      No_Dispatch                              => GNAT0023,
      No_Dispatching_Calls                     => GNAT0024,
      No_Dynamic_Accessibility_Checks          => GNAT0025,
      No_Dynamic_Attachment                    => GNAT0026,
      No_Dynamic_CPU_Assignment                => GNAT0027,
      No_Dynamic_Priorities                    => GNAT0028,
      No_Enumeration_Maps                      => GNAT0029,
      No_Entry_Calls_In_Elaboration_Code       => GNAT0030,
      No_Entry_Queue                           => GNAT0031,
      No_Exception_Handlers                    => GNAT0032,
      No_Exception_Propagation                 => GNAT0033,
      No_Exception_Registration                => GNAT0034,
      No_Exceptions                            => GNAT0035,
      No_Finalization                          => GNAT0036,
      No_Fixed_IO                              => GNAT0037,
      No_Fixed_Point                           => GNAT0038,
      No_Floating_Point                        => GNAT0039,
      No_IO                                    => GNAT0040,
      No_Implicit_Conditionals                 => GNAT0041,
      No_Implicit_Dynamic_Code                 => GNAT0042,
      No_Implicit_Heap_Allocations             => GNAT0043,
      No_Implicit_Task_Allocations             => GNAT0044,
      No_Implicit_Protected_Object_Allocations => GNAT0045,
      No_Initialize_Scalars                    => GNAT0046,
      No_Local_Tagged_Types                    => GNAT0047,
      No_Local_Allocators                      => GNAT0048,
      No_Local_Timing_Events                   => GNAT0049,
      No_Local_Protected_Objects               => GNAT0050,
      No_Long_Long_Integers                    => GNAT0051,
      No_Multiple_Elaboration                  => GNAT0052,
      No_Nested_Finalization                   => GNAT0053,
      No_Protected_Type_Allocators             => GNAT0054,
      No_Protected_Types                       => GNAT0055,
      No_Recursion                             => GNAT0056,
      No_Reentrancy                            => GNAT0057,
      No_Relative_Delay                        => GNAT0058,
      No_Requeue_Statements                    => GNAT0059,
      No_Secondary_Stack                       => GNAT0060,
      No_Select_Statements                     => GNAT0061,
      No_Specific_Termination_Handlers         => GNAT0062,
      No_Standard_Allocators_After_Elaboration => GNAT0063,
      No_Standard_Storage_Pools                => GNAT0064,
      No_Stream_Optimizations                  => GNAT0065,
      No_Streams                               => GNAT0066,
      No_Tagged_Type_Registration              => GNAT0067,
      No_Task_Allocators                       => GNAT0068,
      No_Task_Attributes_Package               => GNAT0069,
      No_Task_At_Interrupt_Priority            => GNAT0070,
      No_Task_Hierarchy                        => GNAT0071,
      No_Task_Hierarchy_Implicit               => GNAT0072,
      No_Task_Termination                      => GNAT0073,
      No_Tasks_Unassigned_To_CPU               => GNAT0074,
      No_Tasking                               => GNAT0075,
      No_Terminate_Alternatives                => GNAT0076,
      No_Unchecked_Access                      => GNAT0077,
      No_Unchecked_Conversion                  => GNAT0078,
      No_Unchecked_Deallocation                => GNAT0079,
      Static_Priorities                        => GNAT0080,
      Static_Storage_Size                      => GNAT0081,

      --  The following require consistency checking with special rules. See
      --  individual routines in unit Bcheck for details of what is required.

      No_Default_Initialization                => GNAT0082,

      --  The following cases do not require consistency checking and if used
      --  as a configuration pragma within a specific unit, apply only to that
      --  unit (e.g. if used in the package spec, do not apply to the body)

      --  Note: No_Elaboration_Code is handled specially. Like the other
      --  non-partition-wide restrictions, it can only be set in a unit that
      --  is part of the extended main source unit (body/spec/subunits). But
      --  it is sticky, in that if it is found anywhere within any of these
      --  units, it applies to all units in this extended main source.

      Immediate_Reclamation                    => GNAT0083,
      No_Dynamic_Sized_Objects                 => GNAT0084,
      No_Implementation_Aspect_Specifications  => GNAT0085,
      No_Implementation_Attributes             => GNAT0086,
      No_Implementation_Identifiers            => GNAT0087,
      No_Implementation_Pragmas                => GNAT0088,
      No_Implementation_Restrictions           => GNAT0089,
      No_Implementation_Units                  => GNAT0090,
      No_Implicit_Aliasing                     => GNAT0091,
      No_Implicit_Loops                        => GNAT0092,
      No_Elaboration_Code                      => GNAT0093,
      No_Obsolescent_Features                  => GNAT0094,
      No_Unrecognized_Aspects                  => GNAT0095,
      No_Unrecognized_Pragmas                  => GNAT0096,
      No_Wide_Characters                       => GNAT0097,
      Static_Dispatch_Tables                   => GNAT0098,
      SPARK_05                                 => GNAT0099,

      --  The following cases require a parameter value

      No_Specification_Of_Aspect               => GNAT0100,
      No_Use_Of_Attribute                      => GNAT0101,
      No_Use_Of_Pragma                         => GNAT0102,

      --  The following entries are fully checked at compile/bind time, which
      --  means that the compiler can in general tell the minimum value which
      --  could be used with a restrictions pragma. The binder can deduce the
      --  appropriate minimum value for the partition by taking the maximum
      --  value required by any unit.

      Max_Protected_Entries                    => GNAT0103,
      Max_Select_Alternatives                  => GNAT0104,
      Max_Task_Entries                         => GNAT0105,

      --  The following entries are also fully checked at compile/bind time,
      --  and the compiler can also at least in some cases tell the minimum
      --  value which could be used with a restriction pragma. The difference
      --  is that the contributions are additive, so the binder deduces this
      --  value by adding the unit contributions.

      Max_Tasks                                => GNAT0106,

      --  The following entries are checked at compile time only for zero/
      --  nonzero entries. This means that the compiler can tell at compile
      --  time if a restriction value of zero is (would be) violated, but that
      --  the compiler cannot distinguish between different non-zero values.

      Max_Asynchronous_Select_Nesting          => GNAT0107,
      Max_Entry_Queue_Length                   => GNAT0108,

      --  The remaining entries are not checked at compile/bind time

      Max_Storage_At_Blocking                  => GNAT0109,

      Not_A_Restriction_Id                     => No_Diagnostic_Id);

end Errid.Restriction_Repository;
