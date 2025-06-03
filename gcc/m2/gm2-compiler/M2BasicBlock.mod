(* M2BasicBlock.mod converts a scope block into a list of basic blocks.

Copyright (C) 2001-2025 Free Software Foundation, Inc.
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

You should have received a copy of the GNU General Public License
along with GNU Modula-2; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  *)

IMPLEMENTATION MODULE M2BasicBlock ;


FROM Storage IMPORT ALLOCATE, DEALLOCATE ;
FROM StrIO IMPORT WriteString, WriteLn ;
FROM NumberIO IMPORT WriteCard ;
FROM M2Debug IMPORT Assert ;
FROM M2Options IMPORT OptimizeBasicBlock ;

FROM M2Quads IMPORT IsReferenced, IsConditional, IsUnConditional, IsCall,
                    IsReturn, IsNewLocalVar, IsKillLocalVar,
                    IsCatchBegin, IsCatchEnd,
                    IsInitStart, IsInitEnd, IsFinallyStart, IsFinallyEnd,
                    IsInitialisingConst, IsConditionalBooleanQuad,
                    IsPseudoQuad, IsDefOrModFile,
                    GetNextQuad, GetQuad, QuadOperator,
                    SubQuad, DisplayQuadRange ;

FROM M2Scope IMPORT ScopeBlock, ForeachScopeBlockDo3 ;


CONST
   Debugging = FALSE ;

TYPE
   BasicBlock = POINTER TO RECORD
                   Scope      : CARDINAL ;  (* Scope associated with the block.  *)
                   StartQuad  : CARDINAL ;  (* First Quad in Basic Block.  *)
                   EndQuad    : CARDINAL ;  (* End Quad in Basic Block.  *)
                   First      : BOOLEAN ;   (* The first block?  *)
                   Right      : BasicBlock ;
                   Left       : BasicBlock ;
                END ;

VAR
   FreeList        : BasicBlock ;   (* Free list of Basic Blocks         *)
   HeadOfBasicBlock: BasicBlock ;


PROCEDURE stop ;
END stop ;


(*
   InitBasicBlocks - converts a list of quadruples as defined by
                     scope blocks into a set of basic blocks.
                     All quadruples within this list which are not
                     reachable are removed.
*)

PROCEDURE InitBasicBlocks (sb: ScopeBlock) : BasicBlock ;
BEGIN
   HeadOfBasicBlock := NIL ;
   ForeachScopeBlockDo3 (sb, ConvertQuads2BasicBlock) ;
   RETURN HeadOfBasicBlock
END InitBasicBlocks ;


(*
   InitBasicBlocksFromRange - converts a list of quadruples as defined by
                              start..end.
                              All quadruples within this list which are not
                              reachable are removed.
*)

PROCEDURE InitBasicBlocksFromRange (ScopeSym: CARDINAL;
                                    start, end: CARDINAL) : BasicBlock ;
BEGIN
   HeadOfBasicBlock := NIL ;
   ConvertQuads2BasicBlock (ScopeSym, start, end) ;
   IF Debugging
   THEN
      DisplayBasicBlocks (HeadOfBasicBlock)
   END ;
   RETURN( HeadOfBasicBlock )
END InitBasicBlocksFromRange ;


(*
   KillBasicBlocks - destroys the list of Basic Blocks.
*)

PROCEDURE KillBasicBlocks (VAR bb: BasicBlock) ;
BEGIN
   FreeBasicBlocks (bb) ;
   bb := NIL
END KillBasicBlocks ;


(*
   FreeBasicBlocks - destroys the list of Basic Blocks.
*)

PROCEDURE FreeBasicBlocks (bb: BasicBlock) ;
VAR
   b, c: BasicBlock ;
BEGIN
   IF bb#NIL
   THEN
      b := bb ;
      REPEAT
         c := bb^.Right ;
         bb^.Right := FreeList ;
         FreeList := bb ;
         bb := c
      UNTIL bb=b
   END
END FreeBasicBlocks ;


(*
   New - returns a basic block.
*)

PROCEDURE New (Scope: CARDINAL; First: BOOLEAN) : BasicBlock ;
VAR
   b: BasicBlock ;
BEGIN
   IF FreeList=NIL
   THEN
      NEW (b)
   ELSE
      b := FreeList ;
      FreeList := FreeList^.Right
   END ;
   Assert (b#NIL) ;
   b^.Scope := Scope ;
   b^.First := First ;
   RETURN b
END New ;


(*
   ConvertQuads2BasicBlock - converts a list of quadruples to a list of
                             Basic Blocks.
                             A Basic Block is defined as a list of quadruples
                             which has only has one entry and exit point.
*)

PROCEDURE ConvertQuads2BasicBlock (ScopeSym: CARDINAL; Start, End: CARDINAL) ;
VAR
   First,
   LastQuadDefMod,
   LastQuadConditional,
   LastQuadCall,
   LastQuadReturn     : BOOLEAN ;
   Quad               : CARDINAL ;
   CurrentBB          : BasicBlock ;
   LastBB             : BasicBlock ;
BEGIN
   IF Debugging
   THEN
      WriteString ("Enter ConvertQuads2BasicBlock") ; WriteLn ;
      DisplayQuadRange (ScopeSym, Start, End)
   END ;
   (*
      Algorithm to perform Basic Block:

      For every quadruple establish a set of leaders.
      A leader is defined as a quadruple which is
      either:

      (i)   The first quadruple.
      (ii)  Any quadruple which is the target of a jump or unconditional jump.
      (iii) Any statement which follows a conditional jump

      For each leader construct a basic block.
      A Basic Block starts with a leader quadruple and ends with either:

      (i)  Another leader
      (ii) An unconditional Jump.

      Any quadruples that do not fall into a Basic Block can be thrown away
      since they will never be executed.
   *)
   LastBB := NIL ;
   CurrentBB := NIL ;
   Quad := Start ;
   LastQuadConditional := TRUE ;  (* Force Rule (i).  *)
   LastQuadCall := FALSE ;
   LastQuadReturn := FALSE ;
   LastQuadDefMod := FALSE ;
   First := TRUE ;
   (* Scan all quadruples.  *)
   WHILE (Quad<=End) AND (Quad#0) DO
      IF Quad = 200
      THEN
         stop
      END ;
      IF LastQuadConditional OR LastQuadCall OR LastQuadReturn OR
         LastQuadDefMod OR IsReferenced(Quad)
      THEN
         (* Rule (ii) *)
         CurrentBB := New (ScopeSym, First) ;  (* Get a new Basic Block.  *)
                                 (* At least one quad in this Basic Block.  *)
         StartBB(CurrentBB, Quad) ;
         EndBB(CurrentBB, Quad) ;
         First := FALSE
      ELSIF CurrentBB#NIL
      THEN
         (* We have a Basic Block - therefore add quad to this Block  *)
         EndBB(CurrentBB, Quad)
      ELSIF IsPseudoQuad(Quad)
      THEN
         (* Add Quad to the Last BB since Pseudo Quads - compiler directives *)
         (* must not be thrown away.                                         *)
         EndBB(LastBB, Quad)
      ELSIF IsReturn(Quad) OR IsKillLocalVar(Quad) OR
            IsCatchEnd(Quad) OR IsCatchBegin(Quad) OR
            IsInitStart(Quad) OR IsInitEnd(Quad) OR
            IsFinallyStart(Quad) OR IsFinallyEnd(Quad)
      THEN
         (* We must leave these quads alone.  *)
         EndBB(LastBB, Quad)
      ELSIF IsConditionalBooleanQuad (Quad)
      THEN
         (* We can remove unreachable const quads.  *)
         SubQuad (Quad)
(*
      ELSIF IsInitialisingConst(Quad)
      THEN
         (* But we leave remaining constant quads alone.  *)
         EndBB(LastBB, Quad)
*)
      ELSE
         (* Remove this Quad since it will never be reached.  *)
         SubQuad(Quad)
      END ;
      LastQuadConditional := IsConditional(Quad) ;
      LastQuadCall := IsCall(Quad) ;
      LastQuadReturn := IsReturn(Quad) ;
      LastQuadDefMod := IsDefOrModFile(Quad) ;
      IF IsUnConditional(Quad)
      THEN
         LastBB := CurrentBB ;
         CurrentBB := NIL
      END ;
      Quad := GetNextQuad(Quad)
   END ;
   IF Debugging
   THEN
      WriteString ("Exit ConvertQuads2BasicBlock") ; WriteLn ;
      DisplayQuadRange (ScopeSym, Start, End)
   END
END ConvertQuads2BasicBlock ;


(*
   ForeachBasicBlockDo - for each basic block call procedure  p.
*)

PROCEDURE ForeachBasicBlockDo (bb: BasicBlock; p: BasicBlockProc) ;
VAR
   b: BasicBlock ;
BEGIN
   IF bb#NIL
   THEN
      b := bb ;
      REPEAT
         WITH b^ DO
            p (b)
         END ;
         b := b^.Right
      UNTIL b=bb
   END
END ForeachBasicBlockDo ;


(*
   StartBB - Initially fills a Basic Block, b, with a start quad Quad.
             The Basic Block is then added to the end of Basic Block list.
*)

PROCEDURE StartBB (b: BasicBlock; Quad: CARDINAL) ;
BEGIN
   WITH b^ DO
      StartQuad := Quad ;
      EndQuad := Quad
   END ;
   Add(HeadOfBasicBlock, b)   (* Add b to the end of the Basic Block list *)
END StartBB ;


(*
   EndBB - Fills a Basic Block, b, with an end quad Quad.
*)

PROCEDURE EndBB (b: BasicBlock; Quad: CARDINAL) ;
BEGIN
   b^.EndQuad := Quad
END EndBB ;


(*
   Add adds a specified element to the end of a queue.
*)

PROCEDURE Add (VAR Head: BasicBlock;
               b : BasicBlock) ;
BEGIN
   IF Head=NIL
   THEN
      Head := b ;
      b^.Left := b ;
      b^.Right := b
   ELSE
      b^.Right := Head ;
      b^.Left := Head^.Left ;
      Head^.Left^.Right := b ;
      Head^.Left := b
   END
END Add ;


(*
   DisplayBasicBlocks - displays the basic block data structure.
*)

PROCEDURE DisplayBasicBlocks (bb: BasicBlock) ;
VAR
   b: BasicBlock ;
BEGIN
   b := bb ;
   WriteString('quadruples') ; WriteLn ;
   IF b#NIL
   THEN
      REPEAT
         DisplayBlock(b) ;
         b := b^.Right
      UNTIL b=bb
   END
END DisplayBasicBlocks ;


PROCEDURE DisplayBlock (b: BasicBlock) ;
BEGIN
   WITH b^ DO
      WriteString(' start ') ; WriteCard(StartQuad, 6) ;
      WriteString(' end   ') ; WriteCard(EndQuad, 6) ;
   END
END DisplayBlock ;


(*
   GetBasicBlockScope - return the scope associated with the basic block.
*)

PROCEDURE GetBasicBlockScope (bb: BasicBlock) : CARDINAL ;
BEGIN
   RETURN bb^.Scope
END GetBasicBlockScope ;


(*
   GetBasicBlockStart - return the quad associated with the start of the basic block.
*)

PROCEDURE GetBasicBlockStart (bb: BasicBlock) : CARDINAL ;
BEGIN
   RETURN bb^.StartQuad
END GetBasicBlockStart ;


(*
   GetBasicBlockEnd - return the quad associated with the end of the basic block.
*)

PROCEDURE GetBasicBlockEnd (bb: BasicBlock) : CARDINAL ;
BEGIN
   RETURN bb^.EndQuad
END GetBasicBlockEnd ;


(*
   IsBasicBlockFirst - return TRUE if this basic block is the first in the sequence.
*)

PROCEDURE IsBasicBlockFirst (bb: BasicBlock) : BOOLEAN ;
BEGIN
   RETURN bb^.First
END IsBasicBlockFirst ;


BEGIN
   FreeList := NIL
END M2BasicBlock.
