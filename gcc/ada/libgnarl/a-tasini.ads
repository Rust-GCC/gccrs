------------------------------------------------------------------------------
--                                                                          --
--                 GNAT RUN-TIME LIBRARY (GNARL) COMPONENTS                 --
--                                                                          --
--              A D A . T A S K _ I N I T I A L I Z A T I O N               --
--                                                                          --
--                                  S p e c                                 --
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

--  This package provides a way to set up a global initialization handler
--  when tasks start.

package Ada.Task_Initialization is
   pragma Preelaborate;
   pragma No_Elaboration_Code_All;

   type Initialization_Handler is access procedure;

   procedure Set_Initialization_Handler (Handler : Initialization_Handler);
   --  Set the global task initialization handler to Handler.
   --  Note that only tasks created after this procedure is called will trigger
   --  a call to Handler. You can use Ada's elaboration rules and pragma
   --  Elaborate_All, or the pragma Linker_Constructor to ensure this
   --  procedure is called early.

private
   pragma Favor_Top_Level (Initialization_Handler);
end Ada.Task_Initialization;
