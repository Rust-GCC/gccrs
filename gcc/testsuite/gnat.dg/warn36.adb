-- { dg-do compile }
-- { dg-options "-gnatwu" }

procedure Warn36 is

  type Arr is array (1 .. 65) of Integer;

  A : Arr;
   
begin
  A := (others => 1);
end;
