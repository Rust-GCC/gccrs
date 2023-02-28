------------------------------------------------------------------------------
--                                                                          --
--                         GNAT LIBRARY COMPONENTS                          --
--                                                                          --
--                        S Y S T E M  . C R C 3 2                          --
--                                                                          --
--                                 S p e c                                  --
--                                                                          --
--          Copyright (C) 2001-2023, Free Software Foundation, Inc.         --
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
-- GNAT was originally developed  by the GNAT team at  New York University. --
-- Extensive contributions were provided by Ada Core Technologies Inc.      --
--                                                                          --
------------------------------------------------------------------------------

--  This package provides routines for computing a commonly used checksum
--  called CRC-32. This is a checksum based on treating the binary data
--  as a polynomial over a binary field, and the exact specifications of
--  the CRC-32 algorithm are as follows:
--
--     Name   : "CRC-32"
--     Width  : 32
--     Poly   : 04C11DB7
--     Init   : FFFFFFFF
--     RefIn  : True
--     RefOut : True
--     XorOut : FFFFFFFF
--     Check  : CBF43926
--
--  Note that this is the algorithm used by PKZip, Ethernet and FDDI.
--
--  For more information about this algorithm see:
--
--  ftp://ftp.rocksoft.com/papers/crc_v3.txt

--  "A Painless Guide to CRC Error Detection Algorithms", Ross N. Williams
--
--  "Computation of Cyclic Redundancy Checks via Table Look-Up", Communications
--  of the ACM, Vol. 31 No. 8, pp.1008-1013 Aug. 1988. Sarwate, D.V.

with Interfaces;

package System.CRC32 is

   type CRC32 is new Interfaces.Unsigned_32;
   --  Used to represent CRC32 values, which are 32 bit bit-strings

   procedure Initialize (C : out CRC32);
   pragma Inline (Initialize);
   --  Initialize CRC value by assigning the standard Init value (16#FFFF_FFFF)

   procedure Update
     (C     : in out CRC32;
      Value : Character);
   pragma Inline (Update);
   --  Evolve CRC by including the contribution from Character'Pos (Value)

   function Get_Value (C : CRC32) return Interfaces.Unsigned_32;
   pragma Inline (Get_Value);
   --  Get_Value computes the CRC32 value by performing an XOR with the
   --  standard XorOut value (16#FFFF_FFFF). Note that this does not
   --  change the value of C, so it may be used to retrieve intermediate
   --  values of the CRC32 value during a sequence of Update calls.

end System.CRC32;
