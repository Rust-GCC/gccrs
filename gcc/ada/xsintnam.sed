#Indicate that the output was generated.
/package Ada.Interrupts.Names/i --  Generated from Ada.Interrupts.Names for use by pragma Interrupt_State

#Disable style checks.
/package Ada.Interrupts.Names/i pragma Style_Checks ("N");

#Turn constants into named numbers.
s/constant Interrupt_ID/constant/g

#Rename the package and add the Preelaborate aspect.
s/package Ada.Interrupts.Names/package System.Interrupt_Names with Preelaborate/g
s/end Ada.Interrupts.Names/end System.Interrupt_Names/g

#Update the copyright header.
/A D A . I N T E R R U P T S . N A M E S/c --                S Y S T E M . I N T E R R U P T S _ N A M E S             --

#The VxWorks and RTEMS versions of Ada.Interrupts.Names define a Hardware_Interrupts
#subtype that references Interrupt_ID. Filter the subtype definition.
/   subtype Hardware_Interrupts is Interrupt_ID/d
/     range Interrupt_ID'First .. System.OS_Interface.Max_HW_Interrupt;/d
/   --  Range of values that can be used for hardware interrupts/d

#The VxWorks and RTEMS versions of Ada.Interrupts.Names also don't define any
#interrupt constants, so just removing the subtype declaration we get an "unused
#with clause" warning. Silence it.
/with System.OS_Interface;/i pragma Warnings (Off);
/with System.OS_Interface;/a pragma Warnings (On);
