------------------------------------------------------------------------------
--                                                                          --
--                 GNAT RUN-TIME LIBRARY (GNARL) COMPONENTS                 --
--                                                                          --
--                     S Y S T E M . I N T E R R U P T S                    --
--                                                                          --
--                                  B o d y                                 --
--                                                                          --
--          Copyright (C) 1998-2019, Free Software Foundation, Inc.         --
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

--  This is the NT version of this package

with Ada.Task_Identification;
with Ada.Unchecked_Conversion;

with Interfaces.C;

with System.Storage_Elements;
with System.Task_Primitives.Operations;
with System.Tasking.Utilities;
with System.Tasking.Rendezvous;
with System.Tasking.Initialization;
with System.Interrupt_Management;
with System.Parameters;

package body System.Interrupts is

   use Parameters;
   use Tasking;
   use System.OS_Interface;
   use Interfaces.C;

   package STPO renames System.Task_Primitives.Operations;
   package IMNG renames System.Interrupt_Management;

   subtype int is Interfaces.C.int;

   function To_System is new Ada.Unchecked_Conversion
     (Ada.Task_Identification.Task_Id, Task_Id);

   type Handler_Kind is (Unknown, Task_Entry, Protected_Procedure);

   type Handler_Desc is record
      Kind   : Handler_Kind := Unknown;
      T      : Task_Id;
      E      : Task_Entry_Index;
      H      : Parameterless_Handler;
      Static : Boolean := False;
   end record;

   task type Server_Task (Interrupt : Interrupt_ID) is
      pragma Interrupt_Priority (System.Interrupt_Priority'Last);
   end Server_Task;

   type Server_Task_Access is access Server_Task;

   Handlers        : array (Interrupt_ID) of Task_Id;
   Descriptors     : array (Interrupt_ID) of Handler_Desc;
   Interrupt_Count : array (Interrupt_ID) of Integer := (others => 0);

   pragma Volatile_Components (Interrupt_Count);

   procedure Attach_Handler
     (New_Handler : Parameterless_Handler;
      Interrupt   : Interrupt_ID;
      Static      : Boolean;
      Restoration : Boolean);
   --  This internal procedure is needed to finalize protected objects that
   --  contain interrupt handlers.

   procedure Signal_Handler (Sig : Interrupt_ID);
   pragma Convention (C, Signal_Handler);
   --  This procedure is used to handle all the signals

   --  Type and Head, Tail of the list containing Registered Interrupt
   --  Handlers. These definitions are used to register the handlers
   --  specified by the pragma Interrupt_Handler.

   --------------------------
   -- Handler Registration --
   --------------------------

   type Registered_Handler;
   type R_Link is access all Registered_Handler;

   type Registered_Handler is record
      H    : System.Address := System.Null_Address;
      Next : R_Link := null;
   end record;

   Registered_Handlers : R_Link := null;

   function Is_Registered (Handler : Parameterless_Handler) return Boolean;
   --  See if the Handler has been "pragma"ed using Interrupt_Handler.
   --  Always consider a null handler as registered.

   type Handler_Ptr is access procedure (Sig : Interrupt_ID);
   pragma Convention (C, Handler_Ptr);

   function TISR is new Ada.Unchecked_Conversion (Handler_Ptr, isr_address);

   --------------------
   -- Signal_Handler --
   --------------------

   procedure Signal_Handler (Sig : Interrupt_ID) is
      Handler : Task_Id renames Handlers (Sig);

   begin
      if Intr_Attach_Reset and then
        intr_attach (int (Sig), TISR (Signal_Handler'Access)) = FUNC_ERR
      then
         raise Program_Error;
      end if;

      if Handler /= null then
         Interrupt_Count (Sig) := Interrupt_Count (Sig) + 1;
         STPO.Wakeup (Handler, Interrupt_Server_Idle_Sleep);
      end if;
   end Signal_Handler;

   -----------------
   -- Is_Reserved --
   -----------------

   function Is_Reserved (Interrupt : Interrupt_ID) return Boolean is
   begin
      return IMNG.Reserve (IMNG.Interrupt_ID (Interrupt));
   end Is_Reserved;

   -----------------------
   -- Is_Entry_Attached --
   -----------------------

   function Is_Entry_Attached (Interrupt : Interrupt_ID) return Boolean is
   begin
      if Is_Reserved (Interrupt) then
         raise Program_Error with
           "interrupt" & Interrupt_ID'Image (Interrupt) & " is reserved";
      end if;

      return Descriptors (Interrupt).T /= Null_Task;
   end Is_Entry_Attached;

   -------------------------
   -- Is_Handler_Attached --
   -------------------------

   function Is_Handler_Attached (Interrupt : Interrupt_ID) return Boolean is
   begin
      if Is_Reserved (Interrupt) then
         raise Program_Error with
           "interrupt" & Interrupt_ID'Image (Interrupt) & " is reserved";
      else
         return Descriptors (Interrupt).Kind /= Unknown;
      end if;
   end Is_Handler_Attached;

   ----------------
   -- Is_Ignored --
   ----------------

   function Is_Ignored (Interrupt : Interrupt_ID) return Boolean is
   begin
      raise Program_Error;
      return False;
   end Is_Ignored;

   ------------------
   -- Unblocked_By --
   ------------------

   function Unblocked_By (Interrupt : Interrupt_ID) return Task_Id is
   begin
      raise Program_Error;
      return Null_Task;
   end Unblocked_By;

   ----------------------
   -- Ignore_Interrupt --
   ----------------------

   procedure Ignore_Interrupt (Interrupt : Interrupt_ID) is
   begin
      raise Program_Error;
   end Ignore_Interrupt;

   ------------------------
   -- Unignore_Interrupt --
   ------------------------

   procedure Unignore_Interrupt (Interrupt : Interrupt_ID) is
   begin
      raise Program_Error;
   end Unignore_Interrupt;

   -------------------------------------
   -- Has_Interrupt_Or_Attach_Handler --
   -------------------------------------

   function Has_Interrupt_Or_Attach_Handler
     (Object : access Dynamic_Interrupt_Protection) return Boolean
   is
      pragma Unreferenced (Object);
   begin
      return True;
   end Has_Interrupt_Or_Attach_Handler;

   --------------
   -- Finalize --
   --------------

   procedure Finalize (Object : in out Static_Interrupt_Protection) is
   begin
      --  ??? loop to be executed only when we're not doing library level
      --  finalization, since in this case all interrupt tasks are gone.

      for N in reverse Object.Previous_Handlers'Range loop
         Attach_Handler
           (New_Handler => Object.Previous_Handlers (N).Handler,
            Interrupt   => Object.Previous_Handlers (N).Interrupt,
            Static      => Object.Previous_Handlers (N).Static,
            Restoration => True);
      end loop;

      Tasking.Protected_Objects.Entries.Finalize
        (Tasking.Protected_Objects.Entries.Protection_Entries (Object));
   end Finalize;

   -------------------------------------
   -- Has_Interrupt_Or_Attach_Handler --
   -------------------------------------

   function Has_Interrupt_Or_Attach_Handler
     (Object : access Static_Interrupt_Protection) return Boolean
   is
      pragma Unreferenced (Object);
   begin
      return True;
   end Has_Interrupt_Or_Attach_Handler;

   ----------------------
   -- Install_Handlers --
   ----------------------

   procedure Install_Handlers
     (Object       : access Static_Interrupt_Protection;
      New_Handlers : New_Handler_Array)
   is
   begin
      for N in New_Handlers'Range loop

         --  We need a lock around this ???

         Object.Previous_Handlers (N).Interrupt := New_Handlers (N).Interrupt;
         Object.Previous_Handlers (N).Static    := Descriptors
           (New_Handlers (N).Interrupt).Static;

         --  We call Exchange_Handler and not directly Interrupt_Manager.
         --  Exchange_Handler so we get the Is_Reserved check.

         Exchange_Handler
           (Old_Handler => Object.Previous_Handlers (N).Handler,
            New_Handler => New_Handlers (N).Handler,
            Interrupt   => New_Handlers (N).Interrupt,
            Static      => True);
      end loop;
   end Install_Handlers;

   ---------------------------------
   -- Install_Restricted_Handlers --
   ---------------------------------

   procedure Install_Restricted_Handlers
      (Prio     : Any_Priority;
       Handlers : New_Handler_Array)
   is
      pragma Unreferenced (Prio);
   begin
      for N in Handlers'Range loop
         Attach_Handler (Handlers (N).Handler, Handlers (N).Interrupt, True);
      end loop;
   end Install_Restricted_Handlers;

   ---------------------
   -- Current_Handler --
   ---------------------

   function Current_Handler
     (Interrupt : Interrupt_ID) return Parameterless_Handler
   is
   begin
      if Is_Reserved (Interrupt) then
         raise Program_Error;
      end if;

      if Descriptors (Interrupt).Kind = Protected_Procedure then
         return Descriptors (Interrupt).H;
      else
         return null;
      end if;
   end Current_Handler;

   --------------------
   -- Attach_Handler --
   --------------------

   procedure Attach_Handler
     (New_Handler : Parameterless_Handler;
      Interrupt   : Interrupt_ID;
      Static      : Boolean := False)
   is
   begin
      Attach_Handler (New_Handler, Interrupt, Static, False);
   end Attach_Handler;

   procedure Attach_Handler
     (New_Handler : Parameterless_Handler;
      Interrupt   : Interrupt_ID;
      Static      : Boolean;
      Restoration : Boolean)
   is
      New_Task : Server_Task_Access;

   begin
      if Is_Reserved (Interrupt) then
         raise Program_Error;
      end if;

      if not Restoration and then not Static

         --  Tries to overwrite a static Interrupt Handler with dynamic handle

        and then
          (Descriptors (Interrupt).Static

            --  New handler not specified as an Interrupt Handler by a pragma

             or else not Is_Registered (New_Handler))
      then
         raise Program_Error with
           "trying to overwrite a static interrupt handler with a " &
           "dynamic handler";
      end if;

      if Handlers (Interrupt) = null then
         New_Task := new Server_Task (Interrupt);
         Handlers (Interrupt) := To_System (New_Task.all'Identity);
      end if;

      if intr_attach (int (Interrupt),
        TISR (Signal_Handler'Access)) = FUNC_ERR
      then
         raise Program_Error;
      end if;

      if New_Handler = null then

         --  The null handler means we are detaching the handler

         Descriptors (Interrupt) :=
           (Kind => Unknown, T => null, E => 0, H => null, Static => False);

      else
         Descriptors (Interrupt).Kind := Protected_Procedure;
         Descriptors (Interrupt).H := New_Handler;
         Descriptors (Interrupt).Static := Static;
      end if;
   end Attach_Handler;

   ----------------------
   -- Exchange_Handler --
   ----------------------

   procedure Exchange_Handler
     (Old_Handler : out Parameterless_Handler;
      New_Handler : Parameterless_Handler;
      Interrupt   : Interrupt_ID;
      Static      : Boolean := False)
   is
   begin
      if Is_Reserved (Interrupt) then
         raise Program_Error;
      end if;

      if Descriptors (Interrupt).Kind = Task_Entry then

         --  In case we have an Interrupt Entry already installed, raise a
         --  program error (propagate it to the caller).

         raise Program_Error with "an interrupt is already installed";

      else
         Old_Handler := Current_Handler (Interrupt);
         Attach_Handler (New_Handler, Interrupt, Static);
      end if;
   end Exchange_Handler;

   --------------------
   -- Detach_Handler --
   --------------------

   procedure Detach_Handler
     (Interrupt : Interrupt_ID;
      Static    : Boolean := False)
   is
   begin
      if Is_Reserved (Interrupt) then
         raise Program_Error;
      end if;

      if Descriptors (Interrupt).Kind = Task_Entry then
         raise Program_Error with "trying to detach an interrupt entry";
      end if;

      if not Static and then Descriptors (Interrupt).Static then
         raise Program_Error with
           "trying to detach a static interrupt handler";
      end if;

      Descriptors (Interrupt) :=
        (Kind => Unknown, T => null, E => 0, H => null, Static => False);

      if intr_attach (int (Interrupt), null) = FUNC_ERR then
         raise Program_Error;
      end if;
   end Detach_Handler;

   ---------------
   -- Reference --
   ---------------

   function Reference (Interrupt : Interrupt_ID) return System.Address is
      Signal : constant System.Address :=
                 System.Storage_Elements.To_Address
                   (System.Storage_Elements.Integer_Address (Interrupt));

   begin
      if Is_Reserved (Interrupt) then

         --  Only usable Interrupts can be used for binding it to an Entry

         raise Program_Error;
      end if;

      return Signal;
   end Reference;

   --------------------------------
   -- Register_Interrupt_Handler --
   --------------------------------

   procedure Register_Interrupt_Handler (Handler_Addr : System.Address) is
   begin
      Registered_Handlers :=
       new Registered_Handler'(H => Handler_Addr, Next => Registered_Handlers);
   end Register_Interrupt_Handler;

   -------------------
   -- Is_Registered --
   -------------------

   --  See if the Handler has been "pragma"ed using Interrupt_Handler.
   --  Always consider a null handler as registered.

   function Is_Registered (Handler : Parameterless_Handler) return Boolean is
      Ptr : R_Link := Registered_Handlers;

      type Acc_Proc is access procedure;

      type Fat_Ptr is record
         Object_Addr  : System.Address;
         Handler_Addr : Acc_Proc;
      end record;

      function To_Fat_Ptr is new Ada.Unchecked_Conversion
        (Parameterless_Handler, Fat_Ptr);

      Fat : Fat_Ptr;

   begin
      if Handler = null then
         return True;
      end if;

      Fat := To_Fat_Ptr (Handler);

      while Ptr /= null loop
         if Ptr.H = Fat.Handler_Addr.all'Address then
            return True;
         end if;

         Ptr := Ptr.Next;
      end loop;

      return False;
   end Is_Registered;

   -----------------------------
   -- Bind_Interrupt_To_Entry --
   -----------------------------

   procedure Bind_Interrupt_To_Entry
     (T       : Task_Id;
      E       : Task_Entry_Index;
      Int_Ref : System.Address)
   is
      Interrupt   : constant Interrupt_ID :=
                      Interrupt_ID (Storage_Elements.To_Integer (Int_Ref));

      New_Task : Server_Task_Access;

   begin
      if Is_Reserved (Interrupt) then
         raise Program_Error;
      end if;

      if Descriptors (Interrupt).Kind /= Unknown then
         raise Program_Error with
           "a binding for this interrupt is already present";
      end if;

      if Handlers (Interrupt) = null then
         New_Task := new Server_Task (Interrupt);
         Handlers (Interrupt) := To_System (New_Task.all'Identity);
      end if;

      if intr_attach (int (Interrupt),
        TISR (Signal_Handler'Access)) = FUNC_ERR
      then
         raise Program_Error;
      end if;

      Descriptors (Interrupt).Kind := Task_Entry;
      Descriptors (Interrupt).T := T;
      Descriptors (Interrupt).E := E;

      --  Indicate the attachment of Interrupt Entry in ATCB. This is needed so
      --  that when an Interrupt Entry task terminates the binding can be
      --  cleaned up. The call to unbinding must be make by the task before it
      --  terminates.

      T.Interrupt_Entry := True;
   end Bind_Interrupt_To_Entry;

   ------------------------------
   -- Detach_Interrupt_Entries --
   ------------------------------

   procedure Detach_Interrupt_Entries (T : Task_Id) is
   begin
      for J in Interrupt_ID loop
         if not Is_Reserved (J) then
            if Descriptors (J).Kind = Task_Entry
              and then Descriptors (J).T = T
            then
               Descriptors (J).Kind := Unknown;

               if intr_attach (int (J), null) = FUNC_ERR then
                  raise Program_Error;
               end if;
            end if;
         end if;
      end loop;

      --  Indicate in ATCB that no Interrupt Entries are attached

      T.Interrupt_Entry := True;
   end Detach_Interrupt_Entries;

   ---------------------
   -- Block_Interrupt --
   ---------------------

   procedure Block_Interrupt (Interrupt : Interrupt_ID) is
   begin
      raise Program_Error;
   end Block_Interrupt;

   -----------------------
   -- Unblock_Interrupt --
   -----------------------

   procedure Unblock_Interrupt (Interrupt : Interrupt_ID) is
   begin
      raise Program_Error;
   end Unblock_Interrupt;

   ----------------
   -- Is_Blocked --
   ----------------

   function Is_Blocked (Interrupt : Interrupt_ID) return Boolean is
   begin
      raise Program_Error;
      return False;
   end Is_Blocked;

   task body Server_Task is
      Ignore : constant Boolean := Utilities.Make_Independent;

      Desc    : Handler_Desc renames Descriptors (Interrupt);
      Self_Id : constant Task_Id := STPO.Self;
      Temp    : Parameterless_Handler;

   begin
      loop
         while Interrupt_Count (Interrupt) > 0 loop
            Interrupt_Count (Interrupt) := Interrupt_Count (Interrupt) - 1;
            begin
               case Desc.Kind is
                  when Unknown =>
                     null;
                  when Task_Entry =>
                     Rendezvous.Call_Simple (Desc.T, Desc.E, Null_Address);
                  when Protected_Procedure =>
                     Temp := Desc.H;
                     Temp.all;
               end case;
            exception
               when others => null;
            end;
         end loop;

         Initialization.Defer_Abort (Self_Id);

         if Single_Lock then
            STPO.Lock_RTS;
         end if;

         STPO.Write_Lock (Self_Id);
         Self_Id.Common.State := Interrupt_Server_Idle_Sleep;
         STPO.Sleep (Self_Id, Interrupt_Server_Idle_Sleep);
         Self_Id.Common.State := Runnable;
         STPO.Unlock (Self_Id);

         if Single_Lock then
            STPO.Unlock_RTS;
         end if;

         Initialization.Undefer_Abort (Self_Id);

         --  Undefer abort here to allow a window for this task to be aborted
         --  at the time of system shutdown.

      end loop;
   end Server_Task;

end System.Interrupts;
