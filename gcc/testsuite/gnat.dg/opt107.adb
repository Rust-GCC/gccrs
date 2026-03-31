-- { dg-do compile }
-- { dg-options "-O" }

with Opt107_Pkg; use Opt107_Pkg;

function Opt107 return Rec is
   R : Rec;
begin
   R.W1A := B_A_7;
   R.W1B := 0;
   R.W1C := C_A;
   R.W1D := (Spare => 0,
             D_A => 0,
             D_B => 0,
             D_C => 0,
             D_D => 0);
   R.W2B := (Spare => 0,
             E_A => 0,
             E_B => 0,
             E_C => 0,
             E_D => 0,
             E_E => 0,
             E_F => 0,
             E_G => 0,
             E_H => 0);
   return R;
end;
