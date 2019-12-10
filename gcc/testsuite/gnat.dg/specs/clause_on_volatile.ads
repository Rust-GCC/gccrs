-- { dg-do compile }

package Clause_On_Volatile is

  type U8 is mod 2 ** 8;

  type Word is record
     A, B : U8;
  end record;
  For Word'Alignment use 4;

  type Vword is new Word;
  For Vword'Alignment use 4;
  pragma Volatile (Vword);

  type Aword is new Word;
  For Aword'Alignment use 4;
  pragma Atomic (Aword);

  type R1 is record
     W : Word;
  end record;
  for R1 use record
     W at 0 range 0 .. 15; -- OK
  end record;

  type A1 is record
     AW : Aword;
  end record;
  For A1'Alignment use 4;
  for A1 use record
     AW at 0 range 0 .. 15; -- { dg-error "must be \[0-9\]*" }
  end record;

  type A2 is record
     B : U8;
     AW : Aword;
  end record;
  For A2'Alignment use 4;
  for A2 use record
     B at 0 range 0 .. 7;
     AW at 1 range 0 .. 31; -- { dg-error "must be multiple|alignment" }
  end record;

  type A3 is record
     B : U8;
     AW : Aword;
  end record;
  For A3'Alignment use 4;
  for A3 use record
     B at 0 range 0 .. 7;
     AW at 1 range 0 .. 15; -- { dg-error "must be (multiple||\[0-9\]*)|alignment" }
  end record;

  type V1 is record
     VW : Vword;
  end record;
  For V1'Alignment use 4;
  for V1 use record
     VW at 0 range 0 .. 15;
  end record;

  type V2 is record
     B : U8;
     VW : Vword;
  end record;
  For V2'Alignment use 4;
  for V2 use record
     B at 0 range 0 .. 7;
     VW at 1 range 0 .. 31;
  end record;

  type V3 is record
     B : U8;
     VW : Vword;
  end record;
  For V3'Alignment use 4;
  for V3 use record
     B at 0 range 0 .. 7;
     VW at 1 range 0 .. 15;
  end record;

end Clause_On_Volatile;
