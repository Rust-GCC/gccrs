with System.Storage_Elements; use System.Storage_Elements;

package Aggr35_Pkg is

  type Rec (D : Storage_Offset := 64) is record
    Data : Storage_Array (0 .. D);
  end record;

  procedure Proc1;

  procedure Proc2;

  procedure Proc3 (R : Rec) is null;

  function Func1 (D : Storage_Offset) return Rec;

  function Func2 (D : Storage_Offset) return Rec;

  function Func3 (D : Storage_Offset) return Rec;

end Aggr35_Pkg;
