package Float_Literal2 is

  I : Integer := 1.1e11111111; -- { dg-error "expected type|found type" }

end Float_Literal2;
