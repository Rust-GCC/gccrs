------------------------------------------------------------------------------
--                                                                          --
--                 GNAT RUN-TIME LIBRARY (GNARL) COMPONENTS                 --
--                                                                          --
--           S Y S T E M . I N T E R R U P T _ M A N A G E M E N T          --
--                                                                          --
--                                  B o d y                                 --
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

--  This is a Solaris version of this package

--  Make a careful study of all signals available under the OS, to see which
--  need to be reserved, kept always unmasked, or kept always unmasked.

--  Be on the lookout for special signals that may be used by the thread
--  library.

package body System.Interrupt_Management is

   use Interfaces.C;
   use System.OS_Interface;

   type Interrupt_List is array (Interrupt_ID range <>) of Interrupt_ID;

   Exception_Interrupts : constant Interrupt_List :=
     (SIGFPE, SIGILL, SIGSEGV, SIGBUS);

   Unreserve_All_Interrupts : constant Interfaces.C.int;
   pragma Import
     (C, Unreserve_All_Interrupts, "__gl_unreserve_all_interrupts");

   function State (Int : Interrupt_ID) return Character;
   pragma Import (C, State, "__gnat_get_interrupt_state");
   --  Get interrupt state.  Defined in init.c
   --  The input argument is the interrupt number,
   --  and the result is one of the following:

   User    : constant Character := 'u';
   Runtime : constant Character := 'r';
   Default : constant Character := 's';
   --    'n'   this interrupt not set by any Interrupt_State pragma
   --    'u'   Interrupt_State pragma set state to User
   --    'r'   Interrupt_State pragma set state to Runtime
   --    's'   Interrupt_State pragma set state to System (use "default"
   --           system handler)

   ----------------------
   -- Notify_Exception --
   ----------------------

   --  This function identifies the Ada exception to be raised using the
   --  information when the system received a synchronous signal. Since this
   --  function is machine and OS dependent, different code has to be provided
   --  for different target.

   procedure Notify_Exception
     (signo   : Signal;
      info    : access siginfo_t;
      context : access ucontext_t);

   ----------------------
   -- Notify_Exception --
   ----------------------

   procedure Notify_Exception
     (signo   : Signal;
      info    : access siginfo_t;
      context : access ucontext_t)
   is
      pragma Unreferenced (info);

   begin
      --  Perform the necessary context adjustments prior to a raise from a
      --  signal handler.

      Adjust_Context_For_Raise (signo, context.all'Address);

      --  Check that treatment of exception propagation here is consistent with
      --  treatment of the abort signal in System.Task_Primitives.Operations.

      case signo is
         when SIGFPE  => raise Constraint_Error;
         when SIGILL  => raise Program_Error;
         when SIGSEGV => raise Storage_Error;
         when SIGBUS  => raise Storage_Error;
         when others  => null;
      end case;
   end Notify_Exception;

   ----------------
   -- Initialize --
   ----------------

   Initialized : Boolean := False;

   procedure Initialize is
      act     : aliased struct_sigaction;
      old_act : aliased struct_sigaction;
      mask    : aliased sigset_t;
      Result  : Interfaces.C.int;

   begin
      if Initialized then
         return;
      end if;

      Initialized := True;

      --  Need to call pthread_init very early because it is doing signal
      --  initializations.

      pthread_init;

      --  Change this if you want to use another signal for task abort.
      --  SIGTERM might be a good one.

      Abort_Task_Interrupt := SIGABRT;

      act.sa_handler := Notify_Exception'Address;

      --  Set sa_flags to SA_NODEFER so that during the handler execution
      --  we do not change the Signal_Mask to be masked for the Signal.
      --  This is a temporary fix to the problem that the Signal_Mask is
      --  not restored after the exception (longjmp) from the handler.
      --  The right fix should be made in sigsetjmp so that we save
      --  the Signal_Set and restore it after a longjmp.

      --  In that case, this field should be changed back to 0. ??? (Dong-Ik)

      act.sa_flags := 16;

      Result := sigemptyset (mask'Access);
      pragma Assert (Result = 0);

      --  ??? For the same reason explained above, we can't mask these signals
      --  because otherwise we won't be able to catch more than one signal.

      act.sa_mask := mask;

      pragma Assert (Keep_Unmasked = (Interrupt_ID'Range => False));
      pragma Assert (Reserve = (Interrupt_ID'Range => False));

      for J in Exception_Interrupts'Range loop
         if State (Exception_Interrupts (J)) /= User then
            Keep_Unmasked (Exception_Interrupts (J)) := True;
            Reserve (Exception_Interrupts (J)) := True;

            if State (Exception_Interrupts (J)) /= Default then
               Result :=
                 sigaction
                 (Signal (Exception_Interrupts (J)), act'Unchecked_Access,
                  old_act'Unchecked_Access);
               pragma Assert (Result = 0);
            end if;
         end if;
      end loop;

      if State (Abort_Task_Interrupt) /= User then
         Keep_Unmasked (Abort_Task_Interrupt) := True;
         Reserve (Abort_Task_Interrupt) := True;
      end if;

      --  Set SIGINT to unmasked state as long as it's
      --  not in "User" state.  Check for Unreserve_All_Interrupts last

      if State (SIGINT) /= User then
         Keep_Unmasked (SIGINT) := True;
         Reserve (SIGINT) := True;
      end if;

      --  Check all signals for state that requires keeping them
      --  unmasked and reserved

      for J in Interrupt_ID'Range loop
         if State (J) = Default or else State (J) = Runtime then
            Keep_Unmasked (J) := True;
            Reserve (J) := True;
         end if;
      end loop;

      --  Add the set of signals that must always be unmasked for this target

      for J in Unmasked'Range loop
         Keep_Unmasked (Interrupt_ID (Unmasked (J))) := True;
         Reserve (Interrupt_ID (Unmasked (J))) := True;
      end loop;

      --  Add target-specific reserved signals

      for J in Reserved'Range loop
         Reserve (Interrupt_ID (Reserved (J))) := True;
      end loop;

      --  Process pragma Unreserve_All_Interrupts. This overrides any
      --  settings due to pragma Interrupt_State:

      if Unreserve_All_Interrupts /= 0 then
         Keep_Unmasked (SIGINT) := False;
         Reserve (SIGINT) := False;
      end if;

      --  We do not have Signal 0 in reality. We just use this value to
      --  identify not existing signals (see s-intnam.ads). Therefore, Signal 0
      --  should not be used in all signal related operations hence mark it as
      --  reserved.

      Reserve (0) := True;
   end Initialize;

end System.Interrupt_Management;
