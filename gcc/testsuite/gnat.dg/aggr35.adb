-- { dg-do run }
-- { dg-options "-gnatws" }

with Aggr35_Pkg; use Aggr35_Pkg;

procedure Aggr35 is
begin

  declare
  begin
    Proc1;
    raise Program_Error with "unreachable code";
  exception
    when Storage_Error =>null;
  end;

  Proc2;

  declare
  begin
    Proc3 (Func1 (32));
    raise Program_Error with "unreachable code";
  exception
    when Storage_Error =>null;
  end;

  Proc3 (Func2 (32));

  Proc3 (Func3 (32));
end;
