------------------------------------------------------------------------------
--                                                                          --
--                 GNAT RUN-TIME LIBRARY (GNARL) COMPONENTS                 --
--                                                                          --
--              A D A . T A S K _ I N I T I A L I Z A T I O N               --
--                                                                          --
--                                  B o d y                                 --
--                                                                          --
--             Copyright (C) 2020-2023, Free Software Foundation, Inc.      --
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
------------------------------------------------------------------------------

package body Ada.Task_Initialization is

   Global_Initialization_Handler : Initialization_Handler := null;
   pragma Atomic (Global_Initialization_Handler);
   pragma Export (Ada, Global_Initialization_Handler,
                  "__gnat_global_initialization_handler");
   --  Global handler called when each task initializes.

   --------------------------------
   -- Set_Initialization_Handler --
   --------------------------------

   procedure Set_Initialization_Handler (Handler : Initialization_Handler) is
   begin
      Global_Initialization_Handler := Handler;
   end Set_Initialization_Handler;

end Ada.Task_Initialization;
