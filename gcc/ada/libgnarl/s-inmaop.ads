------------------------------------------------------------------------------
--                                                                          --
--                 GNAT RUN-TIME LIBRARY (GNARL) COMPONENTS                 --
--                                                                          --
--                  SYSTEM.INTERRUPT_MANAGEMENT.OPERATIONS                  --
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

package System.Interrupt_Management.Operations is

   procedure Thread_Block_Interrupt (Interrupt : Interrupt_ID);
   pragma Inline (Thread_Block_Interrupt);
   --  Mask the calling thread for the interrupt

   procedure Thread_Unblock_Interrupt (Interrupt : Interrupt_ID);
   pragma Inline (Thread_Unblock_Interrupt);
   --  Unmask the calling thread for the interrupt

   procedure Set_Interrupt_Mask (Mask : access Interrupt_Mask);
   --  Set the interrupt mask of the calling thread

   procedure Set_Interrupt_Mask
     (Mask  : access Interrupt_Mask;
      OMask : access Interrupt_Mask);
   pragma Inline (Set_Interrupt_Mask);
   --  Set the interrupt mask of the calling thread while returning the
   --  previous Mask.

   procedure Get_Interrupt_Mask (Mask : access Interrupt_Mask);
   pragma Inline (Get_Interrupt_Mask);
   --  Get the interrupt mask of the calling thread

   function Interrupt_Wait (Mask : access Interrupt_Mask) return Interrupt_ID;
   pragma Inline (Interrupt_Wait);
   --  Wait for the interrupts specified in Mask and return
   --  the interrupt received. Return 0 upon error.

   procedure Install_Default_Action (Interrupt : Interrupt_ID);
   pragma Inline (Install_Default_Action);
   --  Set the sigaction of the Interrupt to default (SIG_DFL)

   procedure Install_Ignore_Action (Interrupt : Interrupt_ID);
   pragma Inline (Install_Ignore_Action);
   --  Set the sigaction of the Interrupt to ignore (SIG_IGN)

   procedure Fill_Interrupt_Mask (Mask : access Interrupt_Mask);
   pragma Inline (Fill_Interrupt_Mask);
   --  Get a Interrupt_Mask with all the interrupt masked

   procedure Empty_Interrupt_Mask (Mask : access Interrupt_Mask);
   pragma Inline (Empty_Interrupt_Mask);
   --  Get a Interrupt_Mask with all the interrupt unmasked

   procedure Add_To_Interrupt_Mask
     (Mask      : access Interrupt_Mask;
      Interrupt : Interrupt_ID);
   pragma Inline (Add_To_Interrupt_Mask);
   --  Mask the given interrupt in the Interrupt_Mask

   procedure Delete_From_Interrupt_Mask
     (Mask      : access Interrupt_Mask;
      Interrupt : Interrupt_ID);
   pragma Inline (Delete_From_Interrupt_Mask);
   --  Unmask the given interrupt in the Interrupt_Mask

   function Is_Member
     (Mask      : access Interrupt_Mask;
      Interrupt : Interrupt_ID) return Boolean;
   pragma Inline (Is_Member);
   --  See if a given interrupt is masked in the Interrupt_Mask

   procedure Copy_Interrupt_Mask (X : out Interrupt_Mask; Y : Interrupt_Mask);
   pragma Inline (Copy_Interrupt_Mask);
   --  Assignment needed for limited private type Interrupt_Mask

   procedure Interrupt_Self_Process (Interrupt : Interrupt_ID);
   pragma Inline (Interrupt_Self_Process);
   --  Raise an Interrupt process-level

   procedure Setup_Interrupt_Mask;
   --  Mask Environment task for all signals
   --  This function should be called by the elaboration of System.Interrupt
   --  to set up proper signal masking in all tasks.

   --  The following objects serve as constants, but are initialized in the
   --  body to aid portability. These should be in System.Interrupt_Management
   --  but since Interrupt_Mask is private type we cannot have them declared
   --  there.

   --  Why not make these deferred constants that are initialized using
   --  function calls in the private part???

   Environment_Mask : aliased Interrupt_Mask;
   --  This mask represents the mask of Environment task when this package is
   --  being elaborated, except the signals being forced to be unmasked by RTS
   --  (items in Keep_Unmasked)

   All_Tasks_Mask : aliased Interrupt_Mask;
   --  This is the mask of all tasks created in RTS. Only one task in RTS
   --  is responsible for masking/unmasking signals (see s-interr.adb).

end System.Interrupt_Management.Operations;
