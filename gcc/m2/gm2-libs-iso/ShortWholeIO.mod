(* ShortWholeIO.mod implements input/output of SHORTINT/SHORTCARD over channels.

Copyright (C) 2013-2023 Free Software Foundation, Inc.
Contributed by Gaius Mulley <gaius.mulley@southwales.ac.uk>.

This file is part of GNU Modula-2.

GNU Modula-2 is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GNU Modula-2 is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

Under Section 7 of GPL version 3, you are granted additional
permissions described in the GCC Runtime Library Exception, version
3.1, as published by the Free Software Foundation.

You should have received a copy of the GNU General Public License and
a copy of the GCC Runtime Library Exception along with this program;
see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
<http://www.gnu.org/licenses/>.  *)

IMPLEMENTATION MODULE ShortWholeIO ;

FROM ConvTypes IMPORT ScanClass, ScanState ;
FROM TextIO IMPORT WriteChar, ReadChar ;
FROM DynamicStrings IMPORT String, char, KillString, Length ;
FROM StringConvert IMPORT IntegerToString, CardinalToString ;
FROM WholeConv IMPORT ScanInt, ScanCard ;
FROM StringChan IMPORT writeString ;
FROM IOConsts IMPORT ReadResults ;


(* Input and output of whole numbers in decimal text form
     over specified channels.  The read result is of the
     type IOConsts.ReadResults.
*)

IMPORT IOChan;

(* The text form of a signed whole number is
     ["+" | "-"], decimal digit, {decimal digit}

   The text form of an unsigned whole number is
     decimal digit, {decimal digit}
*)

PROCEDURE ReadInt (cid: IOChan.ChanId; VAR int: SHORTINT) ;
  (* Skips leading spaces, and removes any remaining characters
     from cid that form part of a signed whole number.  The
     value of this number is assigned to int.  The read result
     is set to the value allRight, outOfRange, wrongFormat,
     endOfLine, or endOfInput.
  *)
VAR
   chClass  : ScanClass ;
   nextState: ScanState ;
   c        : SHORTCARD ;
   ch       : CHAR ;
   negative : BOOLEAN ;
BEGIN
   ReadChar(cid, ch) ;
   negative := FALSE ;
   c := 0 ;
   nextState := ScanInt ;
   REPEAT
      nextState(ch, chClass, nextState) ;
      IF chClass=valid
      THEN
         IF ch='+'
         THEN
            (* ignore *)
         ELSIF ch='-'
         THEN
            negative := NOT negative
         ELSE
            c := c*10+VAL(SHORTCARD, ORD(ch)-ORD('0'))
         END ;
         ReadChar(cid, ch)
      ELSIF chClass=padding
      THEN
         ReadChar(cid, ch)
      END
   UNTIL (chClass=invalid) OR (chClass=terminator) ;
   IF chClass=terminator
   THEN
      IF negative
      THEN
         IF c=VAL(SHORTCARD, MAX(SHORTINT))+1
         THEN
            int := MIN(SHORTINT)
         ELSIF c<=VAL(SHORTCARD, MAX(SHORTINT))
         THEN
            int := -VAL(SHORTINT, c)
         ELSE
            (* overflow *)
            IOChan.SetReadResult(cid, outOfRange)
         END
      ELSE
         int := c
      END
   END
END ReadInt ;


PROCEDURE WriteInt (cid: IOChan.ChanId; int: SHORTINT;
                    width: CARDINAL) ;
  (* Writes the value of int to cid in text form, in a field of
     the given minimum width. *)
VAR
   s: String ;
BEGIN
   s := IntegerToString(int, width, ' ', TRUE, 10, FALSE) ;
   writeString(cid, s) ;
   s := KillString(s)
END WriteInt ;


PROCEDURE ReadCard (cid: IOChan.ChanId; VAR card: SHORTCARD) ;
  (* Skips leading spaces, and removes any remaining characters
     from cid that form part of an unsigned whole number.  The
     value of this number is assigned to card. The read result
     is set to the value allRight, outOfRange, wrongFormat,
     endOfLine, or endOfInput.
  *)
VAR
   chClass  : ScanClass ;
   nextState: ScanState ;
   ch       : CHAR ;
   c        : SHORTCARD ;
BEGIN
   ReadChar(cid, ch) ;
   c := 0 ;
   nextState := ScanCard ;
   REPEAT
      nextState(ch, chClass, nextState) ;
      IF chClass=valid
      THEN
         IF ch='+'
         THEN
            (* ignore *)
         ELSE
            c := c*10+VAL(SHORTCARD, ORD(ch)-ORD('0'))
         END ;
         ReadChar(cid, ch)
      ELSIF chClass=padding
      THEN
         ReadChar(cid, ch)
      END
   UNTIL (chClass=invalid) OR (chClass=terminator) ;
   IF chClass=terminator
   THEN
      card := c
   END
END ReadCard ;


PROCEDURE WriteCard (cid: IOChan.ChanId; card: SHORTCARD;
                     width: CARDINAL);
  (* Writes the value of card to cid in text form, in a field
     of the given minimum width. *)
VAR
   s: String ;
BEGIN
   s := CardinalToString(card, width, ' ', 10, FALSE) ;
   writeString(cid, s) ;
   s := KillString(s)
END WriteCard ;


END ShortWholeIO.
