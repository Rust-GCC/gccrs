-- { dg-do compile }

procedure Generic_Inst21 is

   package Config is
      type Config_Data is private;
   private
      type Config_Data is null record;
   end Config;

   generic
   package Config_Saving is

      type Config_Saver is synchronized interface;

      procedure Register_For_Saving
        (This : in out Config_Saver; Config_Data : Config.Config_Data)
      is abstract;

      type Instance is synchronized new Config_Saver with private;

   private

      protected type Instance is new Config_Saver with
         overriding
         procedure Register_For_Saving (Config_Data : Config.Config_Data);
      end Instance;

   end Config_Saving;

   package body Config_Saving is

      protected body Instance is
         procedure Register_For_Saving (Config_Data : Config.Config_Data) is
         begin
            null;
         end Register_For_Saving;
      end Instance;

   end Config_Saving;

   package My_Config_Saving is new Config_Saving;

   X : aliased My_Config_Saving.Instance;
   Y : access My_Config_Saving.Config_Saver'Class := X'Access;
   Z : Config.Config_Data;

begin
   Y.Register_For_Saving (Z);
end;
