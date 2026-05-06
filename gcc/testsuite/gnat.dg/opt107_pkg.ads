with System;
with Unchecked_Conversion;

package Opt107_Pkg is

   Word_Size : constant := 32;
   type Word_Type is mod 2 ** Word_Size;
   for Word_Type'Size use Word_Size;

   Word : constant := Word_Size / System.Storage_Unit;

   Halfword_Size : constant := Word_Size / 2;
   
   type Halfword_Type is range 0 .. 2 ** Halfword_Size - 1;
   for Halfword_Type'Size use Halfword_Size;

   type One_Bit_Type is mod 2 ** 1;
   for One_Bit_Type'Size use 1;
   
   type Four_Bit_Type is mod 2 ** 4;
   for Four_Bit_Type'Size use 4;

   type Seven_Bit_Type is mod 2 ** 7;
   for Seven_Bit_Type'Size use 7;

   type Eight_Bit_Type is mod 2 ** 8;
   for Eight_Bit_Type'Size use 8;

   type Twelve_Bit_Type is mod 2 ** 12;
   for Twelve_Bit_Type'Size use 12;
   
   type Thirty_One_Bit_Type is mod 2 ** 31;
   for Thirty_One_Bit_Type'Size use 31;

   type W0_Type is record
      A : Eight_Bit_Type;
      B : Eight_Bit_Type;
      C : Halfword_Type;
   end record;

   for W0_Type use record
      A at Word * 0 range 24 .. 31;
      B at Word * 0 range 16 .. 23;
      C at Word * 0 range 0 .. 15;
   end record;
   for W0_Type'Size use Word_Size;

   type A_Type is (A0, A1);
   for A_Type use (A0 => 0, A1 => 1);
   for A_Type'Size use 1;
   
   type B_Type is (B_A_1,
                   B_A_2,
                   B_A_3,
                   B_A_4,
                   B_A_5,
                   B_A_6,
                   B_A_7,
                   B_A_8,
                   B_B_1,
                   B_B_2,
                   B_B_3,
                   B_B_4,
                   B_B_5,
                   B_B_6,
                   B_B_7,
                   B_B_8);

   for B_Type use (B_A_1 => 8#00#,
                   B_A_2 => 8#01#,
                   B_A_3 => 8#02#,
                   B_A_4 => 8#03#,
                   B_A_5 => 8#04#,
                   B_A_6 => 8#05#,
                   B_A_7 => 8#06#,
                   B_A_8 => 8#07#,
                   B_B_1 => 8#10#,
                   B_B_2 => 8#11#,
                   B_B_3 => 8#12#,
                   B_B_4 => 8#13#,
                   B_B_5 => 8#14#,
                   B_B_6 => 8#15#,
                   B_B_7 => 8#16#,
                   B_B_8 => 8#17#);
   for B_Type'Size use 8;
   
   type C_Type is (C_A, C_B);
   for C_Type use (C_A => 0, C_B => 2);
   for C_Type'Size use 4;
   
   type D_Type is record
      Spare : Twelve_Bit_Type;
      D_A : One_Bit_Type;
      D_B : One_Bit_Type;
      D_C : One_Bit_Type;
      D_D : One_Bit_Type;
   end record;

   for D_Type use record
      Spare at 0 range 4 .. 15;
      D_A at 0 range 3 .. 3;
      D_B at 0 range 2 .. 2;
      D_C at 0 range 1 .. 1;
      D_D at 0 range 0 .. 0;
   end record;
   for D_Type'Size use Halfword_Size;

   type E_Type is record
      Spare : Seven_Bit_Type;
      E_A : One_Bit_Type;
      E_B : One_Bit_Type;
      E_C : One_Bit_Type;
      E_D : One_Bit_Type;
      E_E : One_Bit_Type;
      E_F : One_Bit_Type;
      E_G : One_Bit_Type;
      E_H : One_Bit_Type;
   end record;

   for E_Type use record
      Spare at 0 range 8 .. 14;
      E_A at 0 range 7 .. 7;
      E_B at 0 range 6 .. 6;
      E_C at 0 range 5 .. 5;
      E_D at 0 range 4 .. 4;
      E_E at 0 range 3 .. 3;
      E_F at 0 range 2 .. 2;
      E_G at 0 range 1 .. 1;
      E_H at 0 range 0 .. 0;
   end record;
   for E_Type'Size use 15;

   type Rec is record
      W0 : W0_Type;
      W1A : B_Type;
      W1B : Four_Bit_Type;
      W1C : C_Type;
      W1D : D_Type;
      W2A : A_Type;
      W2B : E_Type;
      W2C : Halfword_Type;
   end record;

   for Rec use record
      W0  at Word * 0 range  0 .. 31;
      W1A at Word * 1 range 24 .. 31;
      W1B at Word * 1 range 20 .. 23;
      W1C at Word * 1 range 16 .. 19;
      W1D at Word * 1 range  0 .. 15;
      W2A at Word * 2 range 31 .. 31;
      W2B at Word * 2 range 16 .. 30;
      W2C at Word * 2 range  0 .. 15;
   end record;
   for Rec'Size use 6 * Halfword_Size;

end Opt107_Pkg;
