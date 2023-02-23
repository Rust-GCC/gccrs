------------------------------------------------------------------------------
--                                                                          --
--                 GNAT RUN-TIME LIBRARY (GNARL) COMPONENTS                 --
--                                                                          --
--     S Y S T E M . T A S K _ P R I M I T I V E S . O P E R A T I O N S    --
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

--  This is a NT (native) version of this package

--  This package contains all the GNULL primitives that interface directly with
--  the underlying OS.

with Interfaces.C;
with Interfaces.C.Strings;

with System.Float_Control;
with System.Interrupt_Management;
with System.Multiprocessors;
with System.OS_Primitives;
with System.Task_Info;
with System.Tasking.Debug;
with System.Win32.Ext;

with System.Soft_Links;
--  We use System.Soft_Links instead of System.Tasking.Initialization because
--  the later is a higher level package that we shouldn't depend on. For
--  example when using the restricted run time, it is replaced by
--  System.Tasking.Restricted.Stages.

package body System.Task_Primitives.Operations is

   package SSL renames System.Soft_Links;

   use Interfaces.C;
   use Interfaces.C.Strings;
   use System.OS_Interface;
   use System.OS_Primitives;
   use System.Parameters;
   use System.Task_Info;
   use System.Tasking;
   use System.Tasking.Debug;
   use System.Win32;
   use System.Win32.Ext;

   pragma Link_With ("-Xlinker --stack=0x200000,0x1000");
   --  Change the default stack size (2 MB) for tasking programs on Windows.
   --  This allows about 1000 tasks running at the same time. Note that
   --  we set the stack size for non tasking programs on System unit.
   --  Also note that under Windows XP, we use a Windows XP extension to
   --  specify the stack size on a per task basis, as done under other OSes.

   ---------------------
   -- Local Functions --
   ---------------------

   procedure InitializeCriticalSection (pCriticalSection : access RTS_Lock);
   procedure InitializeCriticalSection
     (pCriticalSection : access CRITICAL_SECTION);
   pragma Import
     (Stdcall, InitializeCriticalSection, "InitializeCriticalSection");

   procedure EnterCriticalSection (pCriticalSection : access RTS_Lock);
   procedure EnterCriticalSection
     (pCriticalSection : access CRITICAL_SECTION);
   pragma Import (Stdcall, EnterCriticalSection, "EnterCriticalSection");

   procedure LeaveCriticalSection (pCriticalSection : access RTS_Lock);
   procedure LeaveCriticalSection (pCriticalSection : access CRITICAL_SECTION);
   pragma Import (Stdcall, LeaveCriticalSection, "LeaveCriticalSection");

   procedure DeleteCriticalSection (pCriticalSection : access RTS_Lock);
   procedure DeleteCriticalSection
     (pCriticalSection : access CRITICAL_SECTION);
   pragma Import (Stdcall, DeleteCriticalSection, "DeleteCriticalSection");

   ----------------
   -- Local Data --
   ----------------

   Environment_Task_Id : Task_Id;
   --  A variable to hold Task_Id for the environment task

   Single_RTS_Lock : aliased RTS_Lock;
   --  This is a lock to allow only one thread of control in the RTS at
   --  a time; it is used to execute in mutual exclusion from all other tasks.
   --  Used to protect All_Tasks_List

   Time_Slice_Val : constant Integer;
   pragma Import (C, Time_Slice_Val, "__gl_time_slice_val");

   Dispatching_Policy : constant Character;
   pragma Import (C, Dispatching_Policy, "__gl_task_dispatching_policy");

   function Get_Policy (Prio : System.Any_Priority) return Character;
   pragma Import (C, Get_Policy, "__gnat_get_specific_dispatching");
   --  Get priority specific dispatching policy

   Foreign_Task_Elaborated : aliased Boolean := True;
   --  Used to identified fake tasks (i.e., non-Ada Threads)

   Null_Thread_Id : constant Thread_Id := 0;
   --  Constant to indicate that the thread identifier has not yet been
   --  initialized.

   ------------------------------------
   -- The thread local storage index --
   ------------------------------------

   TlsIndex : DWORD;
   pragma Export (Ada, TlsIndex);
   --  To ensure that this variable won't be local to this package, since
   --  in some cases, inlining forces this variable to be global anyway.

   --------------------
   -- Local Packages --
   --------------------

   package Specific is

      function Is_Valid_Task return Boolean;
      pragma Inline (Is_Valid_Task);
      --  Does executing thread have a TCB?

      procedure Set (Self_Id : Task_Id);
      pragma Inline (Set);
      --  Set the self id for the current task

   end Specific;

   package body Specific is

      -------------------
      -- Is_Valid_Task --
      -------------------

      function Is_Valid_Task return Boolean is
      begin
         return TlsGetValue (TlsIndex) /= System.Null_Address;
      end Is_Valid_Task;

      ---------
      -- Set --
      ---------

      procedure Set (Self_Id : Task_Id) is
         Succeeded : BOOL;
      begin
         Succeeded := TlsSetValue (TlsIndex, To_Address (Self_Id));
         pragma Assert (Succeeded = Win32.TRUE);
      end Set;

   end Specific;

   ----------------------------------
   -- ATCB allocation/deallocation --
   ----------------------------------

   package body ATCB_Allocation is separate;
   --  The body of this package is shared across several targets

   ---------------------------------
   -- Support for foreign threads --
   ---------------------------------

   function Register_Foreign_Thread
     (Thread         : Thread_Id;
      Sec_Stack_Size : Size_Type := Unspecified_Size) return Task_Id;
   --  Allocate and initialize a new ATCB for the current Thread. The size of
   --  the secondary stack can be optionally specified.

   function Register_Foreign_Thread
     (Thread         : Thread_Id;
      Sec_Stack_Size : Size_Type := Unspecified_Size)
     return Task_Id is separate;

   ----------------------------------
   -- Condition Variable Functions --
   ----------------------------------

   procedure Initialize_Cond (Cond : not null access Condition_Variable);
   --  Initialize given condition variable Cond

   procedure Finalize_Cond (Cond : not null access Condition_Variable);
   --  Finalize given condition variable Cond

   procedure Cond_Signal (Cond : not null access Condition_Variable);
   --  Signal condition variable Cond

   procedure Cond_Wait
     (Cond : not null access Condition_Variable;
      L    : not null access RTS_Lock);
   --  Wait on conditional variable Cond, using lock L

   procedure Cond_Timed_Wait
     (Cond      : not null access Condition_Variable;
      L         : not null access RTS_Lock;
      Rel_Time  : Duration;
      Timed_Out : out Boolean;
      Status    : out Integer);
   --  Do timed wait on condition variable Cond using lock L. The duration
   --  of the timed wait is given by Rel_Time. When the condition is
   --  signalled, Timed_Out shows whether or not a time out occurred.
   --  Status is only valid if Timed_Out is False, in which case it
   --  shows whether Cond_Timed_Wait completed successfully.

   ---------------------
   -- Initialize_Cond --
   ---------------------

   procedure Initialize_Cond (Cond : not null access Condition_Variable) is
      hEvent : HANDLE;
   begin
      hEvent := CreateEvent (null, Win32.TRUE, Win32.FALSE, Null_Ptr);
      pragma Assert (hEvent /= 0);
      Cond.all := Condition_Variable (hEvent);
   end Initialize_Cond;

   -------------------
   -- Finalize_Cond --
   -------------------

   --  No such problem here, DosCloseEventSem has been derived.
   --  What does such refer to in above comment???

   procedure Finalize_Cond (Cond : not null access Condition_Variable) is
      Result : BOOL;
   begin
      Result := CloseHandle (HANDLE (Cond.all));
      pragma Assert (Result = Win32.TRUE);
   end Finalize_Cond;

   -----------------
   -- Cond_Signal --
   -----------------

   procedure Cond_Signal (Cond : not null access Condition_Variable) is
      Result : BOOL;
   begin
      Result := SetEvent (HANDLE (Cond.all));
      pragma Assert (Result = Win32.TRUE);
   end Cond_Signal;

   ---------------
   -- Cond_Wait --
   ---------------

   --  Pre-condition: Cond is posted
   --                 L is locked.

   --  Post-condition: Cond is posted
   --                  L is locked.

   procedure Cond_Wait
     (Cond : not null access Condition_Variable;
      L    : not null access RTS_Lock)
   is
      Result      : DWORD;
      Result_Bool : BOOL;

   begin
      --  Must reset Cond BEFORE L is unlocked

      Result_Bool := ResetEvent (HANDLE (Cond.all));
      pragma Assert (Result_Bool = Win32.TRUE);
      Unlock (L);

      --  No problem if we are interrupted here: if the condition is signaled,
      --  WaitForSingleObject will simply not block

      Result := WaitForSingleObject (HANDLE (Cond.all), Wait_Infinite);
      pragma Assert (Result = 0);

      Write_Lock (L);
   end Cond_Wait;

   ---------------------
   -- Cond_Timed_Wait --
   ---------------------

   --  Pre-condition: Cond is posted
   --                 L is locked.

   --  Post-condition: Cond is posted
   --                  L is locked.

   procedure Cond_Timed_Wait
     (Cond      : not null access Condition_Variable;
      L         : not null access RTS_Lock;
      Rel_Time  : Duration;
      Timed_Out : out Boolean;
      Status    : out Integer)
   is
      Time_Out_Max : constant DWORD := 16#FFFF0000#;
      --  NT 4 can't handle excessive timeout values (e.g. DWORD'Last - 1)

      Time_Out    : DWORD;
      Result      : BOOL;
      Wait_Result : DWORD;

   begin
      --  Must reset Cond BEFORE L is unlocked

      Result := ResetEvent (HANDLE (Cond.all));
      pragma Assert (Result = Win32.TRUE);
      Unlock (L);

      --  No problem if we are interrupted here: if the condition is signaled,
      --  WaitForSingleObject will simply not block.

      if Rel_Time <= 0.0 then
         Timed_Out := True;
         Wait_Result := 0;

      else
         Time_Out :=
           (if Rel_Time >= Duration (Time_Out_Max) / 1000
            then Time_Out_Max
            else DWORD (Rel_Time * 1000));

         Wait_Result := WaitForSingleObject (HANDLE (Cond.all), Time_Out);

         if Wait_Result = WAIT_TIMEOUT then
            Timed_Out := True;
            Wait_Result := 0;
         else
            Timed_Out := False;
         end if;
      end if;

      Write_Lock (L);

      --  Ensure post-condition

      if Timed_Out then
         Result := SetEvent (HANDLE (Cond.all));
         pragma Assert (Result = Win32.TRUE);
      end if;

      Status := Integer (Wait_Result);
   end Cond_Timed_Wait;

   ------------------
   -- Stack_Guard  --
   ------------------

   --  The underlying thread system sets a guard page at the bottom of a thread
   --  stack, so nothing is needed.
   --  ??? Check the comment above

   procedure Stack_Guard (T : ST.Task_Id; On : Boolean) is
      pragma Unreferenced (T, On);
   begin
      null;
   end Stack_Guard;

   --------------------
   -- Get_Thread_Id  --
   --------------------

   function Get_Thread_Id (T : ST.Task_Id) return OSI.Thread_Id is
   begin
      return T.Common.LL.Thread;
   end Get_Thread_Id;

   ----------
   -- Self --
   ----------

   function Self return Task_Id is
      Self_Id : constant Task_Id := To_Task_Id (TlsGetValue (TlsIndex));
   begin
      if Self_Id = null then
         return Register_Foreign_Thread (GetCurrentThread);
      else
         return Self_Id;
      end if;
   end Self;

   ---------------------
   -- Initialize_Lock --
   ---------------------

   --  Note: mutexes and cond_variables needed per-task basis are initialized
   --  in Initialize_TCB and the Storage_Error is handled. Other mutexes (such
   --  as RTS_Lock, Memory_Lock...) used in the RTS is initialized before any
   --  status change of RTS. Therefore raising Storage_Error in the following
   --  routines should be able to be handled safely.

   procedure Initialize_Lock
     (Prio : System.Any_Priority;
      L    : not null access Lock)
   is
   begin
      InitializeCriticalSection (L.Mutex'Access);
      L.Owner_Priority := 0;
      L.Priority := Prio;
   end Initialize_Lock;

   procedure Initialize_Lock
     (L : not null access RTS_Lock; Level : Lock_Level)
   is
      pragma Unreferenced (Level);
   begin
      InitializeCriticalSection (L);
   end Initialize_Lock;

   -------------------
   -- Finalize_Lock --
   -------------------

   procedure Finalize_Lock (L : not null access Lock) is
   begin
      DeleteCriticalSection (L.Mutex'Access);
   end Finalize_Lock;

   procedure Finalize_Lock (L : not null access RTS_Lock) is
   begin
      DeleteCriticalSection (L);
   end Finalize_Lock;

   ----------------
   -- Write_Lock --
   ----------------

   procedure Write_Lock
     (L : not null access Lock; Ceiling_Violation : out Boolean) is
   begin
      L.Owner_Priority := Get_Priority (Self);

      if L.Priority < L.Owner_Priority then
         Ceiling_Violation := True;
         return;
      end if;

      EnterCriticalSection (L.Mutex'Access);

      Ceiling_Violation := False;
   end Write_Lock;

   procedure Write_Lock (L : not null access RTS_Lock) is
   begin
      EnterCriticalSection (L);
   end Write_Lock;

   procedure Write_Lock (T : Task_Id) is
   begin
      EnterCriticalSection (T.Common.LL.L'Access);
   end Write_Lock;

   ---------------
   -- Read_Lock --
   ---------------

   procedure Read_Lock
     (L : not null access Lock; Ceiling_Violation : out Boolean) is
   begin
      Write_Lock (L, Ceiling_Violation);
   end Read_Lock;

   ------------
   -- Unlock --
   ------------

   procedure Unlock (L : not null access Lock) is
   begin
      LeaveCriticalSection (L.Mutex'Access);
   end Unlock;

   procedure Unlock (L : not null access RTS_Lock) is
   begin
      LeaveCriticalSection (L);
   end Unlock;

   procedure Unlock (T : Task_Id) is
   begin
      LeaveCriticalSection (T.Common.LL.L'Access);
   end Unlock;

   -----------------
   -- Set_Ceiling --
   -----------------

   --  Dynamic priority ceilings are not supported by the underlying system

   procedure Set_Ceiling
     (L    : not null access Lock;
      Prio : System.Any_Priority)
   is
      pragma Unreferenced (L, Prio);
   begin
      null;
   end Set_Ceiling;

   -----------
   -- Sleep --
   -----------

   procedure Sleep
     (Self_ID : Task_Id;
      Reason  : System.Tasking.Task_States)
   is
      pragma Unreferenced (Reason);

   begin
      pragma Assert (Self_ID = Self);

      Cond_Wait (Self_ID.Common.LL.CV'Access, Self_ID.Common.LL.L'Access);

      if Self_ID.Deferral_Level = 0
        and then Self_ID.Pending_ATC_Level < Self_ID.ATC_Nesting_Level
      then
         Unlock (Self_ID);
         raise Standard'Abort_Signal;
      end if;
   end Sleep;

   -----------------
   -- Timed_Sleep --
   -----------------

   --  This is for use within the run-time system, so abort is assumed to be
   --  already deferred, and the caller should be holding its own ATCB lock.

   procedure Timed_Sleep
     (Self_ID  : Task_Id;
      Time     : Duration;
      Mode     : ST.Delay_Modes;
      Reason   : System.Tasking.Task_States;
      Timedout : out Boolean;
      Yielded  : out Boolean)
   is
      pragma Unreferenced (Reason);
      Check_Time : Duration := Monotonic_Clock;
      Rel_Time   : Duration;
      Abs_Time   : Duration;

      Result : Integer;

      Local_Timedout : Boolean;

   begin
      Timedout := True;
      Yielded  := False;

      if Mode = Relative then
         Rel_Time := Time;
         Abs_Time := Duration'Min (Time, Max_Sensible_Delay) + Check_Time;
      else
         Rel_Time := Time - Check_Time;
         Abs_Time := Time;
      end if;

      if Rel_Time > 0.0 then
         loop
            exit when Self_ID.Pending_ATC_Level < Self_ID.ATC_Nesting_Level;

            Cond_Timed_Wait
              (Self_ID.Common.LL.CV'Access,
               Self_ID.Common.LL.L'Access,
               Rel_Time, Local_Timedout, Result);
            Check_Time := Monotonic_Clock;

            exit when Abs_Time <= Check_Time;

            if not Local_Timedout then

               --  Somebody may have called Wakeup for us

               Timedout := False;
               exit;
            end if;

            Rel_Time := Abs_Time - Check_Time;
         end loop;
      end if;
   end Timed_Sleep;

   -----------------
   -- Timed_Delay --
   -----------------

   procedure Timed_Delay
     (Self_ID : Task_Id;
      Time    : Duration;
      Mode    : ST.Delay_Modes)
   is
      Check_Time : Duration := Monotonic_Clock;
      Rel_Time   : Duration;
      Abs_Time   : Duration;

      Timedout : Boolean;
      Result   : Integer;

   begin
      Write_Lock (Self_ID);

      if Mode = Relative then
         Rel_Time := Time;
         Abs_Time := Time + Check_Time;
      else
         Rel_Time := Time - Check_Time;
         Abs_Time := Time;
      end if;

      if Rel_Time > 0.0 then
         Self_ID.Common.State := Delay_Sleep;

         loop
            exit when Self_ID.Pending_ATC_Level < Self_ID.ATC_Nesting_Level;

            Cond_Timed_Wait
              (Self_ID.Common.LL.CV'Access,
               Self_ID.Common.LL.L'Access,
               Rel_Time, Timedout, Result);
            Check_Time := Monotonic_Clock;

            exit when Abs_Time <= Check_Time;

            Rel_Time := Abs_Time - Check_Time;
         end loop;

         Self_ID.Common.State := Runnable;
      end if;

      Unlock (Self_ID);
      Yield;
   end Timed_Delay;

   ------------
   -- Wakeup --
   ------------

   procedure Wakeup (T : Task_Id; Reason : System.Tasking.Task_States) is
      pragma Unreferenced (Reason);
   begin
      Cond_Signal (T.Common.LL.CV'Access);
   end Wakeup;

   -----------
   -- Yield --
   -----------

   procedure Yield (Do_Yield : Boolean := True) is
   begin
      --  Note: in a previous implementation if Do_Yield was False, then we
      --  introduced a delay of 1 millisecond in an attempt to get closer to
      --  annex D semantics, and in particular to make ACATS CXD8002 pass. But
      --  this change introduced a huge performance regression evaluating the
      --  Count attribute. So we decided to remove this processing.

      --  Moreover, CXD8002 appears to pass on Windows (although we do not
      --  guarantee full Annex D compliance on Windows in any case).

      if Do_Yield then
         SwitchToThread;
      end if;
   end Yield;

   ------------------
   -- Set_Priority --
   ------------------

   procedure Set_Priority
     (T                   : Task_Id;
      Prio                : System.Any_Priority;
      Loss_Of_Inheritance : Boolean := False)
   is
      Res : BOOL;
      pragma Unreferenced (Loss_Of_Inheritance);

   begin
      Res :=
        SetThreadPriority
          (T.Common.LL.Thread,
           Interfaces.C.int (Underlying_Priorities (Prio)));
      pragma Assert (Res = Win32.TRUE);

      --  Note: Annex D (RM D.2.3(5/2)) requires the task to be placed at the
      --  head of its priority queue when decreasing its priority as a result
      --  of a loss of inherited priority. This is not the case, but we
      --  consider it an acceptable variation (RM 1.1.3(6)), given this is
      --  the built-in behavior offered by the Windows operating system.

      --  In older versions we attempted to better approximate the Annex D
      --  required behavior, but this simulation was not entirely accurate,
      --  and it seems better to live with the standard Windows semantics.

      T.Common.Current_Priority := Prio;
   end Set_Priority;

   ------------------
   -- Get_Priority --
   ------------------

   function Get_Priority (T : Task_Id) return System.Any_Priority is
   begin
      return T.Common.Current_Priority;
   end Get_Priority;

   ----------------
   -- Enter_Task --
   ----------------

   --  There were two paths were we needed to call Enter_Task :
   --  1) from System.Task_Primitives.Operations.Initialize
   --  2) from System.Tasking.Stages.Task_Wrapper

   --  The pseudo handle (LL.Thread) need not be closed when it is no
   --  longer needed. Calling the CloseHandle function with this handle
   --  has no effect.

   procedure Enter_Task (Self_ID : Task_Id) is
      procedure Get_Stack_Bounds (Base : Address; Limit : Address);
      pragma Import (C, Get_Stack_Bounds, "__gnat_get_stack_bounds");
      --  Get stack boundaries
   begin
      Specific.Set (Self_ID);

      --  Properly initializes the FPU for x86 systems

      System.Float_Control.Reset;

      if Self_ID.Common.Task_Info /= null
        and then
          Self_ID.Common.Task_Info.CPU >= CPU_Number (Number_Of_Processors)
      then
         raise Invalid_CPU_Number;
      end if;

      --  Initialize the thread here only if not set. This is done for a
      --  foreign task but is not needed when a real thread-id is already
      --  set in Create_Task. Note that we do want to keep the real thread-id
      --  as it is the only way to free the associated resource. Another way
      --  to say this is that a pseudo thread-id from a foreign thread won't
      --  allow for freeing resources.

      if Self_ID.Common.LL.Thread = Null_Thread_Id then
         Self_ID.Common.LL.Thread := GetCurrentThread;
      end if;

      Self_ID.Common.LL.Thread_Id := GetCurrentThreadId;

      Get_Stack_Bounds
        (Self_ID.Common.Compiler_Data.Pri_Stack_Info.Base'Address,
         Self_ID.Common.Compiler_Data.Pri_Stack_Info.Limit'Address);
   end Enter_Task;

   -------------------
   -- Is_Valid_Task --
   -------------------

   function Is_Valid_Task return Boolean renames Specific.Is_Valid_Task;

   -----------------------------
   -- Register_Foreign_Thread --
   -----------------------------

   function Register_Foreign_Thread return Task_Id is
   begin
      if Is_Valid_Task then
         return Self;
      else
         return Register_Foreign_Thread (GetCurrentThread);
      end if;
   end Register_Foreign_Thread;

   --------------------
   -- Initialize_TCB --
   --------------------

   procedure Initialize_TCB (Self_ID : Task_Id; Succeeded : out Boolean) is
   begin
      --  Initialize thread ID to 0, this is needed to detect threads that
      --  are not yet activated.

      Self_ID.Common.LL.Thread := Null_Thread_Id;

      Initialize_Cond (Self_ID.Common.LL.CV'Access);
      Initialize_Lock (Self_ID.Common.LL.L'Access, ATCB_Level);

      Succeeded := True;
   end Initialize_TCB;

   -----------------
   -- Create_Task --
   -----------------

   procedure Create_Task
     (T          : Task_Id;
      Wrapper    : System.Address;
      Stack_Size : System.Parameters.Size_Type;
      Priority   : System.Any_Priority;
      Succeeded  : out Boolean)
   is
      Initial_Stack_Size : constant := 1024;
      --  We set the initial stack size to 1024. On Windows version prior to XP
      --  there is no way to fix a task stack size. Only the initial stack size
      --  can be set, the operating system will raise the task stack size if
      --  needed.

      function Is_Windows_XP return Integer;
      pragma Import (C, Is_Windows_XP, "__gnat_is_windows_xp");
      --  Returns 1 if running on Windows XP

      hTask          : HANDLE;
      TaskId         : aliased DWORD;
      pTaskParameter : Win32.PVOID;
      Result         : DWORD;
      Entry_Point    : PTHREAD_START_ROUTINE;

      use type System.Multiprocessors.CPU_Range;

   begin
      --  Check whether both Dispatching_Domain and CPU are specified for the
      --  task, and the CPU value is not contained within the range of
      --  processors for the domain.

      if T.Common.Domain /= null
        and then T.Common.Base_CPU /= System.Multiprocessors.Not_A_Specific_CPU
        and then
          (T.Common.Base_CPU not in T.Common.Domain'Range
            or else not T.Common.Domain (T.Common.Base_CPU))
      then
         Succeeded := False;
         return;
      end if;

      pTaskParameter := To_Address (T);

      Entry_Point := To_PTHREAD_START_ROUTINE (Wrapper);

      if Is_Windows_XP = 1 then
         hTask := CreateThread
           (null,
            DWORD (Stack_Size),
            Entry_Point,
            pTaskParameter,
            DWORD (Create_Suspended)
              or DWORD (Stack_Size_Param_Is_A_Reservation),
            TaskId'Unchecked_Access);
      else
         hTask := CreateThread
           (null,
            Initial_Stack_Size,
            Entry_Point,
            pTaskParameter,
            DWORD (Create_Suspended),
            TaskId'Unchecked_Access);
      end if;

      --  Step 1: Create the thread in blocked mode

      if hTask = 0 then
         Succeeded := False;
         return;
      end if;

      --  Step 2: set its TCB

      T.Common.LL.Thread := hTask;

      --  Note: it would be useful to initialize Thread_Id right away to avoid
      --  a race condition in gdb where Thread_ID may not have the right value
      --  yet, but GetThreadId is a Vista specific API, not available under XP:
      --  T.Common.LL.Thread_Id := GetThreadId (hTask); so instead we set the
      --  field to 0 to avoid having a random value. Thread_Id is initialized
      --  in Enter_Task anyway.

      T.Common.LL.Thread_Id := 0;

      --  Step 3: set its priority (child has inherited priority from parent)

      Set_Priority (T, Priority);

      if Time_Slice_Val = 0
        or else Dispatching_Policy = 'F'
        or else Get_Policy (Priority) = 'F'
      then
         --  Here we need Annex D semantics so we disable the NT priority
         --  boost. A priority boost is temporarily given by the system to
         --  a thread when it is taken out of a wait state.

         SetThreadPriorityBoost (hTask, DisablePriorityBoost => Win32.TRUE);
      end if;

      --  Step 4: Handle pragma CPU and Task_Info

      Set_Task_Affinity (T);

      --  Step 5: Now, start it for good

      Result := ResumeThread (hTask);
      pragma Assert (Result = 1);

      Succeeded := Result = 1;
   end Create_Task;

   ------------------
   -- Finalize_TCB --
   ------------------

   procedure Finalize_TCB (T : Task_Id) is
      Succeeded : BOOL;
      pragma Unreferenced (Succeeded);

   begin
      Finalize_Lock (T.Common.LL.L'Access);
      Finalize_Cond (T.Common.LL.CV'Access);

      if T.Known_Tasks_Index /= -1 then
         Known_Tasks (T.Known_Tasks_Index) := null;
      end if;

      if T.Common.LL.Thread /= Null_Thread_Id then

         --  This task has been activated. Close the thread handle. This
         --  is needed to release system resources.

         Succeeded := CloseHandle (T.Common.LL.Thread);
         --  Note that we do not check for the returned value, this is
         --  because the above call will fail for a foreign thread. But
         --  we still need to call it to properly close Ada tasks created
         --  with CreateThread() in Create_Task above.
      end if;

      ATCB_Allocation.Free_ATCB (T);
   end Finalize_TCB;

   ---------------
   -- Exit_Task --
   ---------------

   procedure Exit_Task is
   begin
      Specific.Set (null);
   end Exit_Task;

   ----------------
   -- Abort_Task --
   ----------------

   procedure Abort_Task (T : Task_Id) is
      pragma Unreferenced (T);
   begin
      null;
   end Abort_Task;

   ----------------------
   -- Environment_Task --
   ----------------------

   function Environment_Task return Task_Id is
   begin
      return Environment_Task_Id;
   end Environment_Task;

   --------------
   -- Lock_RTS --
   --------------

   procedure Lock_RTS is
   begin
      Write_Lock (Single_RTS_Lock'Access);
   end Lock_RTS;

   ----------------
   -- Unlock_RTS --
   ----------------

   procedure Unlock_RTS is
   begin
      Unlock (Single_RTS_Lock'Access);
   end Unlock_RTS;

   ----------------
   -- Initialize --
   ----------------

   procedure Initialize (Environment_Task : Task_Id) is
      Discard : BOOL;

   begin
      Environment_Task_Id := Environment_Task;
      OS_Primitives.Initialize;
      Interrupt_Management.Initialize;

      if Time_Slice_Val = 0 or else Dispatching_Policy = 'F' then
         --  Here we need Annex D semantics, switch the current process to the
         --  Realtime_Priority_Class.

         Discard := OS_Interface.SetPriorityClass
                      (GetCurrentProcess, Realtime_Priority_Class);
      end if;

      TlsIndex := TlsAlloc;

      --  Initialize the lock used to synchronize chain of all ATCBs

      Initialize_Lock (Single_RTS_Lock'Access, RTS_Lock_Level);

      Environment_Task.Common.LL.Thread := GetCurrentThread;

      --  Make environment task known here because it doesn't go through
      --  Activate_Tasks, which does it for all other tasks.

      Known_Tasks (Known_Tasks'First) := Environment_Task;
      Environment_Task.Known_Tasks_Index := Known_Tasks'First;

      Enter_Task (Environment_Task);

      --  pragma CPU and dispatching domains for the environment task

      Set_Task_Affinity (Environment_Task);
   end Initialize;

   ---------------------
   -- Monotonic_Clock --
   ---------------------

   function Monotonic_Clock return Duration is
      function Internal_Clock return Duration;
      pragma Import (Ada, Internal_Clock, "__gnat_monotonic_clock");
   begin
      return Internal_Clock;
   end Monotonic_Clock;

   -------------------
   -- RT_Resolution --
   -------------------

   function RT_Resolution return Duration is
      Ticks_Per_Second : aliased LARGE_INTEGER;
   begin
      QueryPerformanceFrequency (Ticks_Per_Second'Access);
      return Duration (1.0 / Ticks_Per_Second);
   end RT_Resolution;

   ----------------
   -- Initialize --
   ----------------

   procedure Initialize (S : in out Suspension_Object) is
   begin
      --  Initialize internal state. It is always initialized to False (ARM
      --  D.10 par. 6).

      S.State := False;
      S.Waiting := False;

      --  Initialize internal mutex

      InitializeCriticalSection (S.L'Access);

      --  Initialize internal condition variable

      S.CV := CreateEvent (null, Win32.TRUE, Win32.FALSE, Null_Ptr);
      pragma Assert (S.CV /= 0);
   end Initialize;

   --------------
   -- Finalize --
   --------------

   procedure Finalize (S : in out Suspension_Object) is
      Result : BOOL;

   begin
      --  Destroy internal mutex

      DeleteCriticalSection (S.L'Access);

      --  Destroy internal condition variable

      Result := CloseHandle (S.CV);
      pragma Assert (Result = Win32.TRUE);
   end Finalize;

   -------------------
   -- Current_State --
   -------------------

   function Current_State (S : Suspension_Object) return Boolean is
   begin
      --  We do not want to use lock on this read operation. State is marked
      --  as Atomic so that we ensure that the value retrieved is correct.

      return S.State;
   end Current_State;

   ---------------
   -- Set_False --
   ---------------

   procedure Set_False (S : in out Suspension_Object) is
   begin
      SSL.Abort_Defer.all;

      EnterCriticalSection (S.L'Access);

      S.State := False;

      LeaveCriticalSection (S.L'Access);

      SSL.Abort_Undefer.all;
   end Set_False;

   --------------
   -- Set_True --
   --------------

   procedure Set_True (S : in out Suspension_Object) is
      Result : BOOL;

   begin
      SSL.Abort_Defer.all;

      EnterCriticalSection (S.L'Access);

      --  If there is already a task waiting on this suspension object then
      --  we resume it, leaving the state of the suspension object to False,
      --  as it is specified in ARM D.10 par. 9. Otherwise, it just leaves
      --  the state to True.

      if S.Waiting then
         S.Waiting := False;
         S.State := False;

         Result := SetEvent (S.CV);
         pragma Assert (Result = Win32.TRUE);

      else
         S.State := True;
      end if;

      LeaveCriticalSection (S.L'Access);

      SSL.Abort_Undefer.all;
   end Set_True;

   ------------------------
   -- Suspend_Until_True --
   ------------------------

   procedure Suspend_Until_True (S : in out Suspension_Object) is
      Result      : DWORD;
      Result_Bool : BOOL;

   begin
      SSL.Abort_Defer.all;

      EnterCriticalSection (S.L'Access);

      if S.Waiting then

         --  Program_Error must be raised upon calling Suspend_Until_True
         --  if another task is already waiting on that suspension object
         --  (ARM D.10 par. 10).

         LeaveCriticalSection (S.L'Access);

         SSL.Abort_Undefer.all;

         raise Program_Error;

      else
         --  Suspend the task if the state is False. Otherwise, the task
         --  continues its execution, and the state of the suspension object
         --  is set to False (ARM D.10 par. 9).

         if S.State then
            S.State := False;

            LeaveCriticalSection (S.L'Access);

            SSL.Abort_Undefer.all;

         else
            S.Waiting := True;

            --  Must reset CV BEFORE L is unlocked

            Result_Bool := ResetEvent (S.CV);
            pragma Assert (Result_Bool = Win32.TRUE);

            LeaveCriticalSection (S.L'Access);

            SSL.Abort_Undefer.all;

            Result := WaitForSingleObject (S.CV, Wait_Infinite);
            pragma Assert (Result = 0);
         end if;
      end if;
   end Suspend_Until_True;

   ----------------
   -- Check_Exit --
   ----------------

   --  Dummy versions, currently this only works for solaris (native)

   function Check_Exit (Self_ID : ST.Task_Id) return Boolean is
      pragma Unreferenced (Self_ID);
   begin
      return True;
   end Check_Exit;

   --------------------
   -- Check_No_Locks --
   --------------------

   function Check_No_Locks (Self_ID : ST.Task_Id) return Boolean is
      pragma Unreferenced (Self_ID);
   begin
      return True;
   end Check_No_Locks;

   ------------------
   -- Suspend_Task --
   ------------------

   function Suspend_Task
     (T           : ST.Task_Id;
      Thread_Self : Thread_Id) return Boolean
   is
   begin
      if T.Common.LL.Thread /= Thread_Self then
         return SuspendThread (T.Common.LL.Thread) = NO_ERROR;
      else
         return True;
      end if;
   end Suspend_Task;

   -----------------
   -- Resume_Task --
   -----------------

   function Resume_Task
     (T           : ST.Task_Id;
      Thread_Self : Thread_Id) return Boolean
   is
   begin
      if T.Common.LL.Thread /= Thread_Self then
         return ResumeThread (T.Common.LL.Thread) = NO_ERROR;
      else
         return True;
      end if;
   end Resume_Task;

   --------------------
   -- Stop_All_Tasks --
   --------------------

   procedure Stop_All_Tasks is
   begin
      null;
   end Stop_All_Tasks;

   ---------------
   -- Stop_Task --
   ---------------

   function Stop_Task (T : ST.Task_Id) return Boolean is
      pragma Unreferenced (T);
   begin
      return False;
   end Stop_Task;

   -------------------
   -- Continue_Task --
   -------------------

   function Continue_Task (T : ST.Task_Id) return Boolean is
      pragma Unreferenced (T);
   begin
      return False;
   end Continue_Task;

   -----------------------
   -- Set_Task_Affinity --
   -----------------------

   procedure Set_Task_Affinity (T : ST.Task_Id) is
      Result : DWORD;

      use type System.Multiprocessors.CPU_Range;

   begin
      --  Do nothing if the underlying thread has not yet been created. If the
      --  thread has not yet been created then the proper affinity will be set
      --  during its creation.

      if T.Common.LL.Thread = Null_Thread_Id then
         null;

      --  pragma CPU

      elsif T.Common.Base_CPU /= Multiprocessors.Not_A_Specific_CPU then

         --  The CPU numbering in pragma CPU starts at 1 while the subprogram
         --  to set the affinity starts at 0, therefore we must substract 1.

         Result :=
           SetThreadIdealProcessor
             (T.Common.LL.Thread, ProcessorId (T.Common.Base_CPU) - 1);
         pragma Assert (Result = 1);

      --  Task_Info

      elsif T.Common.Task_Info /= null then
         if T.Common.Task_Info.CPU /= Task_Info.Any_CPU then
            Result :=
              SetThreadIdealProcessor
                (T.Common.LL.Thread, T.Common.Task_Info.CPU);
            pragma Assert (Result = 1);
         end if;

      --  Dispatching domains

      elsif T.Common.Domain /= null
        and then (T.Common.Domain /= ST.System_Domain
                   or else
                     T.Common.Domain.all /=
                       (Multiprocessors.CPU'First ..
                        Multiprocessors.Number_Of_CPUs => True))
      then
         declare
            CPU_Set : DWORD := 0;

         begin
            for Proc in T.Common.Domain'Range loop
               if T.Common.Domain (Proc) then

                  --  The thread affinity mask is a bit vector in which each
                  --  bit represents a logical processor.

                  CPU_Set := CPU_Set + 2 ** (Integer (Proc) - 1);
               end if;
            end loop;

            Result := SetThreadAffinityMask (T.Common.LL.Thread, CPU_Set);
            pragma Assert (Result = 1);
         end;
      end if;
   end Set_Task_Affinity;

end System.Task_Primitives.Operations;
