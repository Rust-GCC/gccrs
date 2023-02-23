------------------------------------------------------------------------------
--                                                                          --
--                  GNAT RUN-TIME LIBRARY (GNARL) COMPONENTS                --
--                                                                          --
--                 S Y S T E M . T A S K I N G . Q U E U I N G              --
--                                                                          --
--                                  B o d y                                 --
--                                                                          --
--         Copyright (C) 1992-2023, Free Software Foundation, Inc.          --
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

--  This version of the body implements queueing policy according to the policy
--  specified by the pragma Queuing_Policy. When no such pragma is specified
--  FIFO policy is used as default.

with System.Task_Primitives.Operations;
with System.Tasking.Initialization;

package body System.Tasking.Queuing is

   use Task_Primitives.Operations;
   use Protected_Objects;
   use Protected_Objects.Entries;

   --  Entry Queues implemented as doubly linked list

   Queuing_Policy : constant Character;
   pragma Import (C, Queuing_Policy, "__gl_queuing_policy");

   Priority_Queuing : constant Boolean := Queuing_Policy = 'P';

   procedure Send_Program_Error
     (Self_ID    : Task_Id;
      Entry_Call : Entry_Call_Link);
   --  Raise Program_Error in the caller of the specified entry call

   function Check_Queue (E : Entry_Queue) return Boolean;
   --  Check the validity of E.
   --  Return True if E is valid, raise Assert_Failure if assertions are
   --  enabled and False otherwise.

   -----------------------------
   -- Broadcast_Program_Error --
   -----------------------------

   procedure Broadcast_Program_Error
     (Self_ID      : Task_Id;
      Object       : Protection_Entries_Access;
      Pending_Call : Entry_Call_Link)
   is
      Entry_Call : Entry_Call_Link;
   begin
      if Pending_Call /= null then
         Send_Program_Error (Self_ID, Pending_Call);
      end if;

      for E in Object.Entry_Queues'Range loop
         Dequeue_Head (Object.Entry_Queues (E), Entry_Call);

         while Entry_Call /= null loop
            pragma Assert (Entry_Call.Mode /= Conditional_Call);

            Send_Program_Error (Self_ID, Entry_Call);
            Dequeue_Head (Object.Entry_Queues (E), Entry_Call);
         end loop;
      end loop;
   end Broadcast_Program_Error;

   -----------------
   -- Check_Queue --
   -----------------

   function Check_Queue (E : Entry_Queue) return Boolean is
      Valid   : Boolean := True;
      C, Prev : Entry_Call_Link;

   begin
      if E.Head = null then
         if E.Tail /= null then
            Valid := False;
            pragma Assert (Valid);
         end if;
      else
         if E.Tail = null
           or else E.Tail.Next /= E.Head
         then
            Valid := False;
            pragma Assert (Valid);

         else
            C := E.Head;

            loop
               Prev := C;
               C := C.Next;

               if C = null then
                  Valid := False;
                  pragma Assert (Valid);
                  exit;
               end if;

               if Prev /= C.Prev then
                  Valid := False;
                  pragma Assert (Valid);
                  exit;
               end if;

               exit when C = E.Head;
            end loop;

            if Prev /= E.Tail then
               Valid := False;
               pragma Assert (Valid);
            end if;
         end if;
      end if;

      return Valid;
   end Check_Queue;

   -------------------
   -- Count_Waiting --
   -------------------

   --  Return number of calls on the waiting queue of E

   function Count_Waiting (E : Entry_Queue) return Natural is
      Count   : Natural;
      Temp    : Entry_Call_Link;

   begin
      pragma Assert (Check_Queue (E));

      Count := 0;

      if E.Head /= null then
         Temp := E.Head;

         loop
            Count := Count + 1;
            exit when E.Tail = Temp;
            Temp := Temp.Next;
         end loop;
      end if;

      return Count;
   end Count_Waiting;

   -------------
   -- Dequeue --
   -------------

   --  Dequeue call from entry_queue E

   procedure Dequeue (E : in out Entry_Queue; Call : Entry_Call_Link) is
   begin
      pragma Assert (Check_Queue (E));
      pragma Assert (Call /= null);

      --  If empty queue, simply return

      if E.Head = null then
         return;
      end if;

      pragma Assert (Call.Prev /= null);
      pragma Assert (Call.Next /= null);

      Call.Prev.Next := Call.Next;
      Call.Next.Prev := Call.Prev;

      if E.Head = Call then

         --  Case of one element

         if E.Tail = Call then
            E.Head := null;
            E.Tail := null;

         --  More than one element

         else
            E.Head := Call.Next;
         end if;

      elsif E.Tail = Call then
         E.Tail := Call.Prev;
      end if;

      --  Successfully dequeued

      Call.Prev := null;
      Call.Next := null;
      pragma Assert (Check_Queue (E));
   end Dequeue;

   ------------------
   -- Dequeue_Call --
   ------------------

   procedure Dequeue_Call (Entry_Call : Entry_Call_Link) is
      Called_PO : Protection_Entries_Access;

   begin
      pragma Assert (Entry_Call /= null);

      if Entry_Call.Called_Task /= null then
         Dequeue
           (Entry_Call.Called_Task.Entry_Queues
             (Task_Entry_Index (Entry_Call.E)),
           Entry_Call);

      else
         Called_PO := To_Protection (Entry_Call.Called_PO);
         Dequeue (Called_PO.Entry_Queues
             (Protected_Entry_Index (Entry_Call.E)),
           Entry_Call);
      end if;
   end Dequeue_Call;

   ------------------
   -- Dequeue_Head --
   ------------------

   --  Remove and return the head of entry_queue E

   procedure Dequeue_Head
     (E    : in out Entry_Queue;
      Call : out Entry_Call_Link)
   is
      Temp : Entry_Call_Link;

   begin
      pragma Assert (Check_Queue (E));
      --  If empty queue, return null pointer

      if E.Head = null then
         Call := null;
         return;
      end if;

      Temp := E.Head;

      --  Case of one element

      if E.Head = E.Tail then
         E.Head := null;
         E.Tail := null;

      --  More than one element

      else
         pragma Assert (Temp /= null);
         pragma Assert (Temp.Next /= null);
         pragma Assert (Temp.Prev /= null);

         E.Head := Temp.Next;
         Temp.Prev.Next := Temp.Next;
         Temp.Next.Prev := Temp.Prev;
      end if;

      --  Successfully dequeued

      Temp.Prev := null;
      Temp.Next := null;
      Call := Temp;
      pragma Assert (Check_Queue (E));
   end Dequeue_Head;

   -------------
   -- Enqueue --
   -------------

   --  Enqueue call at the end of entry_queue E, for FIFO queuing policy.
   --  Enqueue call priority ordered, FIFO at same priority level, for
   --  Priority queuing policy.

   procedure Enqueue (E : in out Entry_Queue; Call : Entry_Call_Link) is
      Temp : Entry_Call_Link := E.Head;

   begin
      pragma Assert (Check_Queue (E));
      pragma Assert (Call /= null);

      --  Priority Queuing

      if Priority_Queuing then
         if Temp = null then
            Call.Prev := Call;
            Call.Next := Call;
            E.Head := Call;
            E.Tail := Call;

         else
            loop
               --  Find the entry that the new guy should precede

               exit when Call.Prio > Temp.Prio;
               Temp := Temp.Next;

               if Temp = E.Head then
                  Temp := null;
                  exit;
               end if;
            end loop;

            if Temp = null then
               --  Insert at tail

               Call.Prev := E.Tail;
               Call.Next := E.Head;
               E.Tail := Call;

            else
               Call.Prev := Temp.Prev;
               Call.Next := Temp;

               --  Insert at head

               if Temp = E.Head then
                  E.Head := Call;
               end if;
            end if;

            pragma Assert (Call.Prev /= null);
            pragma Assert (Call.Next /= null);

            Call.Prev.Next := Call;
            Call.Next.Prev := Call;
         end if;

         pragma Assert (Check_Queue (E));
         return;
      end if;

      --  FIFO Queuing

      if E.Head = null then
         E.Head := Call;
      else
         E.Tail.Next := Call;
         Call.Prev   := E.Tail;
      end if;

      E.Head.Prev := Call;
      E.Tail      := Call;
      Call.Next   := E.Head;
      pragma Assert (Check_Queue (E));
   end Enqueue;

   ------------------
   -- Enqueue_Call --
   ------------------

   procedure Enqueue_Call (Entry_Call : Entry_Call_Link) is
      Called_PO : Protection_Entries_Access;

   begin
      pragma Assert (Entry_Call /= null);

      if Entry_Call.Called_Task /= null then
         Enqueue
           (Entry_Call.Called_Task.Entry_Queues
              (Task_Entry_Index (Entry_Call.E)),
           Entry_Call);

      else
         Called_PO := To_Protection (Entry_Call.Called_PO);
         Enqueue (Called_PO.Entry_Queues
             (Protected_Entry_Index (Entry_Call.E)),
           Entry_Call);
      end if;
   end Enqueue_Call;

   ----------
   -- Head --
   ----------

   --  Return the head of entry_queue E

   function Head (E : Entry_Queue) return Entry_Call_Link is
   begin
      pragma Assert (Check_Queue (E));
      return E.Head;
   end Head;

   -------------
   -- Onqueue --
   -------------

   --  Return True if Call is on any entry_queue at all

   function Onqueue (Call : Entry_Call_Link) return Boolean is
   begin
      pragma Assert (Call /= null);

      --  Utilize the fact that every queue is circular, so if Call
      --  is on any queue at all, Call.Next must NOT be null.

      return Call.Next /= null;
   end Onqueue;

   --------------------------------
   -- Requeue_Call_With_New_Prio --
   --------------------------------

   procedure Requeue_Call_With_New_Prio
     (Entry_Call : Entry_Call_Link; Prio : System.Any_Priority) is
   begin
      pragma Assert (Entry_Call /= null);

      --  Perform a queue reordering only when the policy being used is the
      --  Priority Queuing.

      if Priority_Queuing then
         if Onqueue (Entry_Call) then
            Dequeue_Call (Entry_Call);
            Entry_Call.Prio := Prio;
            Enqueue_Call (Entry_Call);
         end if;
      end if;
   end Requeue_Call_With_New_Prio;

   ---------------------------------
   -- Select_Protected_Entry_Call --
   ---------------------------------

   --  Select an entry of a protected object. Selection depends on the
   --  queuing policy being used.

   procedure Select_Protected_Entry_Call
     (Self_ID : Task_Id;
      Object  : Protection_Entries_Access;
      Call    : out Entry_Call_Link)
   is
      Entry_Call  : Entry_Call_Link;
      Temp_Call   : Entry_Call_Link;
      Entry_Index : Protected_Entry_Index := Null_Entry; -- stop warning

   begin
      Entry_Call := null;

      begin
         --  Priority queuing case

         if Priority_Queuing then
            for J in Object.Entry_Queues'Range loop
               Temp_Call := Head (Object.Entry_Queues (J));

               if Temp_Call /= null
                 and then
                   Object.Entry_Bodies
                     (Object.Find_Body_Index
                       (Object.Compiler_Info, J)).
                          Barrier (Object.Compiler_Info, J)
               then
                  if Entry_Call = null
                    or else Entry_Call.Prio < Temp_Call.Prio
                  then
                     Entry_Call := Temp_Call;
                     Entry_Index := J;
                  end if;
               end if;
            end loop;

         --  FIFO queueing case

         else
            for J in Object.Entry_Queues'Range loop
               Temp_Call := Head (Object.Entry_Queues (J));

               if Temp_Call /= null
                 and then
                   Object.Entry_Bodies
                     (Object.Find_Body_Index
                       (Object.Compiler_Info, J)).
                          Barrier (Object.Compiler_Info, J)
               then
                  Entry_Call := Temp_Call;
                  Entry_Index := J;
                  exit;
               end if;
            end loop;
         end if;

      exception
         when others =>
            Broadcast_Program_Error (Self_ID, Object, null);
      end;

      --  If a call was selected, dequeue it and return it for service

      if Entry_Call /= null then
         Temp_Call := Entry_Call;
         Dequeue_Head (Object.Entry_Queues (Entry_Index), Entry_Call);
         pragma Assert (Temp_Call = Entry_Call);
      end if;

      Call := Entry_Call;
   end Select_Protected_Entry_Call;

   ----------------------------
   -- Select_Task_Entry_Call --
   ----------------------------

   --  Select an entry for rendezvous. Selection depends on the queuing policy
   --  being used.

   procedure Select_Task_Entry_Call
     (Acceptor         : Task_Id;
      Open_Accepts     : Accept_List_Access;
      Call             : out Entry_Call_Link;
      Selection        : out Select_Index;
      Open_Alternative : out Boolean)
   is
      Entry_Call  : Entry_Call_Link;
      Temp_Call   : Entry_Call_Link;
      Entry_Index : Task_Entry_Index := Task_Entry_Index'First;
      Temp_Entry  : Task_Entry_Index;

   begin
      Open_Alternative := False;
      Entry_Call       := null;
      Selection        := No_Rendezvous;

      if Priority_Queuing then
         --  Priority queueing case

         for J in Open_Accepts'Range loop
            Temp_Entry := Open_Accepts (J).S;

            if Temp_Entry /= Null_Task_Entry then
               Open_Alternative := True;
               Temp_Call := Head (Acceptor.Entry_Queues (Temp_Entry));

               if Temp_Call /= null
                 and then (Entry_Call = null
                   or else Entry_Call.Prio < Temp_Call.Prio)
               then
                  Entry_Call  := Head (Acceptor.Entry_Queues (Temp_Entry));
                  Entry_Index := Temp_Entry;
                  Selection := J;
               end if;
            end if;
         end loop;

      else
         --  FIFO Queuing case

         for J in Open_Accepts'Range loop
            Temp_Entry := Open_Accepts (J).S;

            if Temp_Entry /= Null_Task_Entry then
               Open_Alternative := True;
               Temp_Call := Head (Acceptor.Entry_Queues (Temp_Entry));

               if Temp_Call /= null then
                  Entry_Call := Head (Acceptor.Entry_Queues (Temp_Entry));
                  Entry_Index := Temp_Entry;
                  Selection := J;
                  exit;
               end if;
            end if;
         end loop;
      end if;

      if Entry_Call /= null then
         Dequeue_Head (Acceptor.Entry_Queues (Entry_Index), Entry_Call);

         --  Guard is open
      end if;

      Call := Entry_Call;
   end Select_Task_Entry_Call;

   ------------------------
   -- Send_Program_Error --
   ------------------------

   procedure Send_Program_Error
     (Self_ID    : Task_Id;
      Entry_Call : Entry_Call_Link)
   is
      Caller : Task_Id;
   begin
      Caller := Entry_Call.Self;
      Entry_Call.Exception_To_Raise := Program_Error'Identity;
      Write_Lock (Caller);
      Initialization.Wakeup_Entry_Caller (Self_ID, Entry_Call, Done);
      Unlock (Caller);
   end Send_Program_Error;

end System.Tasking.Queuing;
