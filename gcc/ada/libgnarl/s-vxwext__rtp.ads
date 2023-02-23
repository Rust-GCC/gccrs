------------------------------------------------------------------------------
--                                                                          --
--                  GNAT RUN-TIME LIBRARY (GNARL) COMPONENTS                --
--                                                                          --
--                     S Y S T E M . V X W O R K S . E X T                  --
--                                                                          --
--                                   S p e c                                --
--                                                                          --
--            Copyright (C) 2008-2023, Free Software Foundation, Inc.       --
--                                                                          --
-- GNARL is free software;  you can redistribute it  and/or modify it under --
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
------------------------------------------------------------------------------

--  This package provides vxworks specific support functions needed
--  by System.OS_Interface.

--  This is the VxWorks 6 RTP version of this package

with Interfaces.C;

package System.VxWorks.Ext is
   pragma Preelaborate;

   subtype SEM_ID is Long_Integer;
   --  typedef struct semaphore *SEM_ID;

   type sigset_t is mod 2 ** Long_Long_Integer'Size;

   type t_id is new Long_Integer;
   subtype int is Interfaces.C.int;
   subtype unsigned is Interfaces.C.unsigned;

   type STATUS is new int;
   --  Equivalent of the C type STATUS

   OK    : constant STATUS := 0;
   ERROR : constant STATUS := -1;

   type BOOL is new int;
   --  Equivalent of the C type BOOL

   type vx_freq_t is new unsigned;
   --  Equivalent of the C type _Vx_freq_t

   type Interrupt_Handler is access procedure (parameter : System.Address);
   pragma Convention (C, Interrupt_Handler);

   type Interrupt_Vector is new System.Address;

   function Int_Lock return int;
   pragma Inline (Int_Lock);

   procedure Int_Unlock (Old : int);
   pragma Inline (Int_Unlock);

   function Interrupt_Connect
     (Vector    : Interrupt_Vector;
      Handler   : Interrupt_Handler;
      Parameter : System.Address := System.Null_Address) return STATUS;
   pragma Convention (C, Interrupt_Connect);

   function Interrupt_Context return BOOL;
   pragma Convention (C, Interrupt_Context);

   function Interrupt_Number_To_Vector
     (intNum : int) return Interrupt_Vector;
   pragma Convention (C, Interrupt_Number_To_Vector);

   function semDelete (Sem : SEM_ID) return STATUS;
   pragma Convention (C, semDelete);

   function Task_Cont (tid : t_id) return STATUS;
   pragma Import (C, Task_Cont, "taskResume");

   function Task_Stop (tid : t_id) return STATUS;
   pragma Import (C, Task_Stop, "taskSuspend");

   function kill (pid : t_id; sig : int) return int;
   pragma Import (C, kill, "taskKill");

   function getpid return t_id;
   pragma Import (C, getpid, "getpid");

   function Set_Time_Slice (ticks : int) return STATUS;
   pragma Inline (Set_Time_Slice);

   --------------------------------
   -- Processor Affinity for SMP --
   --------------------------------

   function taskCpuAffinitySet (tid : t_id; CPU : int) return int;
   pragma Convention (C, taskCpuAffinitySet);
   --  For SMP run-times set the CPU affinity.
   --  For uniprocessor systems return ERROR status.

   function taskMaskAffinitySet (tid : t_id; CPU_Set : unsigned) return int;
   pragma Convention (C, taskMaskAffinitySet);
   --  For SMP run-times set the CPU mask affinity.
   --  For uniprocessor systems return ERROR status.

end System.VxWorks.Ext;
