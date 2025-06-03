------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--               U N C H E C K E D _ D E A L L O C A T I O N                --
--                                                                          --
--                                 S p e c                                  --
--                                                                          --
-- This specification is derived from the Ada Reference Manual for use with --
-- GNAT.  In accordance with the copyright of that document, you can freely --
-- copy and modify this specification,  provided that if you redistribute a --
-- modified version,  any changes that you have made are clearly indicated. --
--                                                                          --
------------------------------------------------------------------------------

--  Renaming of :ref:`Ada.Unchecked_Deallocation`

generic
   type Object (<>) is limited private;
   type Name is access Object;

procedure Unchecked_Deallocation (X : in out Name);
pragma Import (Intrinsic, Unchecked_Deallocation);
