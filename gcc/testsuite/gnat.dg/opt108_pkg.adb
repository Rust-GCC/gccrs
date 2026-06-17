with Interfaces;

package body Opt108_Pkg is

   procedure Put is
   begin
      if Val.X /= 1 then
         raise Program_Error;
      end if;
   end;

   procedure Set is
      type Array_Type is array (Index_Type) of Test_Type;

      New_Val : Array_Type := (others => (X => 0));

      procedure Next
        (Index :        Index_Type;
         T     : in out Test_Type)
      is
      begin
         T := (X => T.X + 1);
         New_Val (Index) := T;
      end Next;

      Cur : Test_Type;

   begin
      Cur := (X => 0);

      if not Flag then
         Next (Index => Cur_Index, T => Cur);
      end if;

      Val := New_Val (1);
   end;

end Opt108_Pkg;
