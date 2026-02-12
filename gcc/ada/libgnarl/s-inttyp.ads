with System.OS_Interface;

package System.Interrupt_Types
  with Preelaborate
is
   type Preelab_Interrupt_ID is range 0 .. System.OS_Interface.Max_Interrupt;
end System.Interrupt_Types;
