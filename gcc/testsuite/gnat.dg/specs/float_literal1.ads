package Float_Literal1 is

  F : Float := 1.1e11111111111; -- { dg-error "not in range|fails" }

end Float_Literal1;
