------------------------------------------------------------------------------
--                                                                          --
--                 GNAT RUN-TIME LIBRARY (GNARL) COMPONENTS                 --
--                                                                          --
--            S Y S T E M . I N T E R R U P T _ M A N A G E M E N T         --
--                                                                          --
--                                  S p e c                                 --
--                                                                          --
--          Copyright (C) 1992-2023, Free Software Foundation, Inc.         --
--                                                                          --
-- GNARL is free software; you can  redistribute it  and/or modify it under --
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
-- GNARL was developed by the GNARL team at Florida State University.       --
-- Extensive contributions were provided by Ada Core Technologies, Inc.     --
--                                                                          --
------------------------------------------------------------------------------

--  This is the VxWorks version of this package

--  This package encapsulates and centralizes information about all
--  uses of interrupts (or signals), including the target-dependent
--  mapping of interrupts (or signals) to exceptions.

--  Unlike the original design, System.Interrupt_Management can only
--  be used for tasking systems.

--  PLEASE DO NOT put any subprogram declarations with arguments of
--  type Interrupt_ID into the visible part of this package. The type
--  Interrupt_ID is used to derive the type in Ada.Interrupts, and
--  adding more operations to that type would be illegal according
--  to the Ada Reference Manual. This is the reason why the signals
--  sets are implemented using visible arrays rather than functions.

with System.OS_Interface;

with Interfaces.C;

package System.Interrupt_Management is
   pragma Preelaborate;

   type Interrupt_Mask is limited private;

   type Interrupt_ID is new Interfaces.C.int
     range 0 .. System.OS_Interface.Max_Interrupt;

   type Interrupt_Set is array (Interrupt_ID) of Boolean;

   subtype Signal_ID is Interrupt_ID range 0 .. System.OS_Interface.NSIG - 1;

   type Signal_Set is array (Signal_ID) of Boolean;

   --  The following objects serve as constants, but are initialized in the
   --  body to aid portability. This permits us to use more portable names for
   --  interrupts, where distinct names may map to the same interrupt ID
   --  value.

   --  For example, suppose SIGRARE is a signal that is not defined on all
   --  systems, but is always reserved when it is defined. If we have the
   --  convention that ID zero is not used for any "real" signals, and SIGRARE
   --  = 0 when SIGRARE is not one of the locally supported signals, we can
   --  write:
   --     Reserved (SIGRARE) := True;
   --  and the initialization code will be portable.

   Abort_Task_Interrupt : Signal_ID;
   --  The signal that is used to implement task abort if an interrupt is used
   --  for that purpose. This is one of the reserved signals.

   Reserve : Interrupt_Set := (others => False);
   --  Reserve (I) is true iff the interrupt I is one that cannot be permitted
   --  to be attached to a user handler. The possible reasons are many. For
   --  example, it may be mapped to an exception used to implement task abort,
   --  or used to implement time delays.

   procedure Initialize_Interrupts;
   pragma Import (C, Initialize_Interrupts, "__gnat_install_handler");
   --  Under VxWorks, there is no signal inheritance between tasks.
   --  This procedure is used to initialize signal-to-exception mapping in
   --  each task.

   procedure Initialize;
   --  Initialize the various variables defined in this package. This procedure
   --  must be called before accessing any object from this package and can be
   --  called multiple times (only the first call has any effect).

private
   type Interrupt_Mask is new System.OS_Interface.sigset_t;
   --  In some implementation Interrupt_Mask can be represented as a linked
   --  list.

end System.Interrupt_Management;
