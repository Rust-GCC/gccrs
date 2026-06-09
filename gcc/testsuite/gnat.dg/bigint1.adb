-- { dg-do run }
-- { dg-options "-gnat2022 -gnata" }

with Ada.Numerics.Big_Numbers.Big_Integers;
use Ada.Numerics.Big_Numbers.Big_Integers;

procedure Bigint1 is

  Minus_One   : constant Big_Integer := To_Big_Integer (-1);
  Minus_Two   : constant Big_Integer := To_Big_Integer (-2);
  Minus_Three : constant Big_Integer := To_Big_Integer (-3);

begin

  pragma Assert (Minus_One ** 1 = Minus_One);
  pragma Assert (Minus_One ** 2 = To_Big_Integer (1));
  pragma Assert (Minus_One ** 3 = Minus_One);

  pragma Assert (Minus_Two ** 1 = Minus_Two);
  pragma Assert (Minus_Two ** 2 = To_Big_Integer (4));
  pragma Assert (Minus_Two ** 3 = To_Big_Integer (-8));

  pragma Assert (Minus_Three ** 1 = Minus_Three);
  pragma Assert (Minus_Three ** 2 = To_Big_Integer (9));
  pragma Assert (Minus_Three ** 3 = To_Big_Integer (-27));

end;
