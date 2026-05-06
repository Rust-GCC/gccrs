--  { dg-do compile }
--  { dg-options "-gnatc" }

package Component_Size1 is

  type T1 is delta 2.0**(-8) range 0.0 .. 1.0 - 2.0**(-8);

  type Arr1 is array (Boolean) of T1
    with Component_Size => 1; -- { dg-error "too small" }

  type Rec1 is record
    Comp : T1;
  end record;
  for Rec1 use record
    Comp at 0 range 0 .. 0; -- { dg-error "too small" }
  end record;

  type T2 is array (1 .. 8) of Boolean with Pack => True;

  type Arr2 is array (Boolean) of T2
    with Component_Size => 1; -- { dg-error "too small" }

  type Rec2 is record
    Comp : T2;
  end record;
  for Rec2 use record
    Comp at 0 range 0 .. 0; -- { dg-error "too small" }
  end record;

end Component_Size1;
