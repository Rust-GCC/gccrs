#Indicate that the output was generated.
/package Ada.Interrupts.Names/i --  Generated from Ada.Interrupts.Names for use by pragma Interrupt_State

#Turn constants into named numbers.
s/constant Interrupt_ID/constant/g

#Rename the package and add the Preelaborate aspect.
s/package Ada.Interrupts.Names/package System.Interrupt_Names with Preelaborate/g
s/end Ada.Interrupts.Names/end System.Interrupt_Names/g

#Update the copyright header.
/A D A . I N T E R R U P T S . N A M E S/c --                S Y S T E M . I N T E R R U P T S _ N A M E S             --

#Remove non-copyright-header comments.
s/[[:blank:]]*--  [^ ].*//g
