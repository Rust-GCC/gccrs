------------------------------------------------------------------------------
--                                                                          --
--                         GNAT LIBRARY COMPONENTS                          --
--                                                                          --
--                   G N A T . S E C U R E _ H A S H E S                    --
--                                                                          --
--                                 S p e c                                  --
--                                                                          --
--          Copyright (C) 2009-2023, Free Software Foundation, Inc.         --
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

--  This package provides common supporting code for a family of secure
--  hash functions (including MD5 and the FIPS PUB 180-3 functions SHA-1,
--  SHA-224, SHA-256, SHA-384 and SHA-512).

--  This is an internal unit and should be not used directly in applications.
--  Use GNAT.MD5 and GNAT.SHA* instead.

with Ada.Streams; use Ada.Streams;

with Interfaces;

with System;

package GNAT.Secure_Hashes is

   type Buffer_Type is new Stream_Element_Array;
   for Buffer_Type'Alignment use 8;
   --  Secure hash functions use a string buffer that is also accessed as an
   --  array of words, which may require up to 64 bit alignment.

   --  The function-independent part of processing state: A buffer of data
   --  being accumulated until a complete block is ready for hashing.

   type Message_State (Block_Length : Stream_Element_Count) is record
      Last : Stream_Element_Offset := 0;
      --  Index of last used element in Buffer

      Length : Interfaces.Unsigned_64 := 0;
      --  Total length of processed data

      Buffer : Buffer_Type (1 .. Block_Length);
      --  Data buffer
   end record;

   --  The function-specific part of processing state:

   --  Each hash function maintains an internal state as an array of words,
   --  which is ultimately converted to a stream representation with the
   --  appropriate bit order.

   generic
      type Word is mod <>;
      --  Either 32 or 64 bits

      with procedure Swap (X : System.Address);
      --  Byte swapping function for a Word at X

      Hash_Bit_Order : System.Bit_Order;
      --  Bit order of the produced hash

   package Hash_Function_State is

      type State is array (Stream_Element_Offset range <>) of Word;
      --  Used to store a hash function's internal state

      procedure To_Hash
        (H      : State;
         H_Bits : out Stream_Element_Array);
      --  Convert H to stream representation with the given bit order. If
      --  H_Bits is smaller than the internal hash state, then the state
      --  is truncated.

   end Hash_Function_State;

   --  Generic hashing framework: The user interface for each implemented
   --  secure hash function is an instance of this generic package.

   generic
      Block_Words : Stream_Element_Count;
      --  Number of words in each block

      State_Words : Stream_Element_Count;
      --  Number of words in internal state

      Hash_Words : Stream_Element_Count;
      --  Number of words in the final hash (must be no greater than
      --  State_Words).

      Hash_Bit_Order : System.Bit_Order;
      --  Bit order used for conversion between bit representation and word
      --  representation.

      with package Hash_State is new Hash_Function_State (<>);
      --  Hash function state package

      Initial_State : Hash_State.State;
      --  Initial value of the hash function state

      with procedure Transform
        (H : in out Hash_State.State;
         M : in out Message_State);
      --  Transformation function updating H by processing a complete data
      --  block from M.

   package H is

      --  The visible part of H is the interface to secure hashing functions
      --  that is exposed to user applications, and is intended to remain
      --  a stable interface.

      pragma Assert (Hash_Words <= State_Words);

      type Context is private;
      --  The internal processing state of the hashing function

      function "=" (L, R : Context) return Boolean is abstract;
      --  Context is the internal, implementation defined intermediate state
      --  in a hash computation, and no specific semantics can be expected on
      --  equality of context values. Only equality of final hash values (as
      --  returned by the [Wide_]Digest functions below) is meaningful.

      Initial_Context : constant Context;
      --  Initial value of a Context object. May be used to reinitialize
      --  a Context value by simple assignment of this value to the object.

      function HMAC_Initial_Context (Key : String) return Context;
      --  Initial Context for HMAC computation with the given Key

      procedure Update      (C : in out Context; Input : String);
      procedure Wide_Update (C : in out Context; Input : Wide_String);
      procedure Update
        (C     : in out Context;
         Input : Stream_Element_Array);
      --  Update C to process the given input. Successive calls to Update are
      --  equivalent to a single call with the concatenation of the inputs. For
      --  the Wide_String version, each Wide_Character is processed low order
      --  byte first.

      Word_Length : constant Stream_Element_Offset := Hash_State.Word'Size / 8;
      Hash_Length : constant Stream_Element_Offset := Hash_Words * Word_Length;

      subtype Binary_Message_Digest is Stream_Element_Array (1 .. Hash_Length);
      --  The fixed-length byte array returned by Digest, providing
      --  the hash in binary representation.

      function Digest (C : Context) return Binary_Message_Digest;
      --  Return hash or HMAC for the data accumulated with C

      function Digest      (S : String)      return Binary_Message_Digest;
      function Wide_Digest (W : Wide_String) return Binary_Message_Digest;
      function Digest
        (A : Stream_Element_Array)           return Binary_Message_Digest;
      --  These functions are equivalent to the corresponding Update (or
      --  Wide_Update) on a default initialized Context, followed by Digest
      --  on the resulting Context.

      subtype Message_Digest is String (1 .. 2 * Integer (Hash_Length));
      --  The fixed-length string returned by Digest, providing the hash in
      --  hexadecimal representation.

      function Digest (C : Context) return Message_Digest;
      --  Return hash or HMAC for the data accumulated with C in hexadecimal
      --  representation.

      function Digest      (S : String)               return Message_Digest;
      function Wide_Digest (W : Wide_String)          return Message_Digest;
      function Digest      (A : Stream_Element_Array) return Message_Digest;
      --  These functions are equivalent to the corresponding Update (or
      --  Wide_Update) on a default initialized Context, followed by Digest
      --  on the resulting Context.

      type Hash_Stream (C : access Context) is
        new Root_Stream_Type with private;
      --  Stream wrapper converting Write calls to Update calls on C.
      --  Arbitrary data structures can thus be conveniently hashed using
      --  their stream attributes.

   private

      Block_Length : constant Stream_Element_Count :=
         Block_Words * Word_Length;
      --  Length in bytes of a data block

      subtype Key_Length is
        Stream_Element_Offset range 0 .. Block_Length;

      --  KL is 0 for a normal hash context, > 0 for HMAC

      type Context (KL : Key_Length := 0) is record
         H_State : Hash_State.State (0 .. State_Words - 1) := Initial_State;
         --  Function-specific state

         M_State : Message_State (Block_Length);
         --  Function-independent state (block buffer)

         Key : Stream_Element_Array (1 .. KL);
         --  HMAC key
      end record;

      pragma Warnings (Off, "aggregate not fully initialized");
      Initial_Context : constant Context (KL => 0) := (others => <>);
      pragma Warnings (On, "aggregate not fully initialized");
      --  Initial values are provided by default initialization of Context

      type Hash_Stream (C : access Context) is
        new Root_Stream_Type with null record;

      procedure Read
        (Stream : in out Hash_Stream;
         Item   : out Stream_Element_Array;
         Last   : out Stream_Element_Offset);
      --  Raise Program_Error: hash streams are write-only

      procedure Write
         (Stream : in out Hash_Stream;
          Item   : Stream_Element_Array);
      --  Call Update

   end H;

end GNAT.Secure_Hashes;
