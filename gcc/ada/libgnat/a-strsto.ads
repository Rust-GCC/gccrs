------------------------------------------------------------------------------
--                                                                          --
--                         GNAT LIBRARY COMPONENTS                          --
--                                                                          --
--                   A D A . S T R E A M S . S T O R A G E                  --
--                                                                          --
--                                 S p e c                                  --
--                                                                          --
--             Copyright (C) 2020-2023, Free Software Foundation, Inc.      --
--                                                                          --
-- This specification is derived from the Ada Reference Manual for use with --
-- GNAT. The copyright notice above, and the license provisions that follow --
-- apply solely to the  contents of the part following the private keyword. --
--                                                                          --
-- GNAT is free software;  you can  redistribute it  and/or modify it under --
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

package Ada.Streams.Storage with Pure is

   type Storage_Stream_Type is abstract new Root_Stream_Type with private;

   function Element_Count
     (Stream : Storage_Stream_Type) return Stream_Element_Count is abstract;

   procedure Clear (Stream : in out Storage_Stream_Type) is abstract;

private
   type Storage_Stream_Type is abstract new Root_Stream_Type with null record;
end Ada.Streams.Storage;
