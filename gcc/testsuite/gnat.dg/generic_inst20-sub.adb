-- { dg-excess-errors "cannot generate code" }

separate (Generic_Inst20)

package body Sub is
   function F return Boolean is
   begin
      return True;
   end F;
   generic
   package G is
   end G;
   package I is new G;
end Sub;
