package body Aggr35_Pkg is

  procedure Proc1 is
    R : Rec := (D => 32, Data => (others => 0)); -- mutable => too large
  begin
    null;
  end;

  procedure Proc2 is
    R : constant Rec := (D => 32, Data => (others => 0)); -- immutable
  begin
    null;
  end;

  function Func1 (D : Storage_Offset) return Rec is
  begin
    return R : Rec := (D => D, Data => (others => 0)); -- mutable => too large
  end;

  function Func2 (D : Storage_Offset) return Rec is
  begin
    return R : constant Rec := (D => D, Data => (others => 0)); -- immutable
  end;

  function Func3 (D : Storage_Offset) return Rec is
  begin
    return (D => D, Data => (others => 0)); -- immutable
  end;

end Aggr35_Pkg;
