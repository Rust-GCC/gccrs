-- { dg-do compile }
-- { dg-options "-gnatwu" }

procedure Warn35 is

  type Bit is ('0', '1');

  type Bit_Array is array (1 .. 2) of Bit;

  Bits : constant Bit_Array := ("01");

  B : Bit;

begin
  B := Bits (1);
end;
