--  { dg-do compile }

procedure Asynch2 is

  procedure Proc is null;

begin
  select
    Proc; -- { dg-error "must be procedure or entry call or delay statement" }
  then abort
    loop
      delay 1.0;
    end loop;
  end select;
end;
