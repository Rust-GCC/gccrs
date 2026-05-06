-- { dg-do run }

with Ada.Directories; use Ada.Directories;

procedure Directory1 is

  procedure Test (Path : in String) is
    With_Slash : constant String := Path & '/';
  begin
    if Containing_Directory (With_Slash) /= Containing_Directory (Path) then
      raise Program_Error;
    end if;
  end;

begin
  Test ("/a/b");
  Test ("a/b");
  Test ("b");
end;
