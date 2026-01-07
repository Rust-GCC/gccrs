------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                                E R R I D                                 --
--                                                                          --
--                                 S p e c                                  --
--                                                                          --
--          Copyright (C) 1992-2026, Free Software Foundation, Inc.         --
--                                                                          --
-- GNAT is free software;  you can  redistribute it  and/or modify it under --
-- terms of the  GNU General Public License as published  by the Free Soft- --
-- ware  Foundation;  either version 3,  or (at your option) any later ver- --
-- sion.  GNAT is distributed in the hope that it will be useful, but WITH- --
-- OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY --
-- or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License --
-- for  more details.  You should have  received  a copy of the GNU General --
-- Public License  distributed with GNAT; see file COPYING3.  If not, go to --
-- http://www.gnu.org/licenses for a complete copy of the license.          --
--                                                                          --
-- GNAT was originally developed  by the GNAT team at  New York University. --
-- Extensive contributions were provided by Ada Core Technologies Inc.      --
--                                                                          --
------------------------------------------------------------------------------

package Errid is

   type Status_Type is
     (Active,
      Deprecated);

   type Diagnostic_Id is
     (No_Diagnostic_Id,
      GNAT0001,
      GNAT0002,
      GNAT0003,
      GNAT0004,
      GNAT0005,
      GNAT0006,
      GNAT0007,
      GNAT0008,
      GNAT0009,
      GNAT0010,
      GNAT0011,
      GNAT0012,
      GNAT0013,
      GNAT0014,
      GNAT0015,
      GNAT0016,
      GNAT0017,
      GNAT0018,
      GNAT0019,
      GNAT0020,
      GNAT0021,
      GNAT0022,
      GNAT0023,
      GNAT0024,
      GNAT0025,
      GNAT0026,
      GNAT0027,
      GNAT0028,
      GNAT0029,
      GNAT0030,
      GNAT0031,
      GNAT0032,
      GNAT0033,
      GNAT0034,
      GNAT0035,
      GNAT0036,
      GNAT0037,
      GNAT0038,
      GNAT0039,
      GNAT0040,
      GNAT0041,
      GNAT0042,
      GNAT0043,
      GNAT0044,
      GNAT0045,
      GNAT0046,
      GNAT0047,
      GNAT0048,
      GNAT0049,
      GNAT0050,
      GNAT0051,
      GNAT0052,
      GNAT0053,
      GNAT0054,
      GNAT0055,
      GNAT0056,
      GNAT0057,
      GNAT0058,
      GNAT0059,
      GNAT0060,
      GNAT0061,
      GNAT0062,
      GNAT0063,
      GNAT0064,
      GNAT0065,
      GNAT0066,
      GNAT0067,
      GNAT0068,
      GNAT0069,
      GNAT0070,
      GNAT0071,
      GNAT0072,
      GNAT0073,
      GNAT0074,
      GNAT0075,
      GNAT0076,
      GNAT0077,
      GNAT0078,
      GNAT0079,
      GNAT0080,
      GNAT0081,
      GNAT0082,
      GNAT0083,
      GNAT0084,
      GNAT0085,
      GNAT0086,
      GNAT0087,
      GNAT0088,
      GNAT0089,
      GNAT0090,
      GNAT0091,
      GNAT0092,
      GNAT0093,
      GNAT0094,
      GNAT0095,
      GNAT0096,
      GNAT0097,
      GNAT0098,
      GNAT0099,
      GNAT0100,
      GNAT0101,
      GNAT0102,
      GNAT0103,
      GNAT0104,
      GNAT0105,
      GNAT0106,
      GNAT0107,
      GNAT0108,
      GNAT0109);

   type Diagnostic_Id_Array is array (Positive range <>) of Diagnostic_Id;
   type Diagnostic_Id_Array_Access is access Diagnostic_Id_Array;
   type Switch_Id is (
      No_Switch_Id,
      gnatel,
      gnatwb,
      gnatwc,
      gnatwd,
      gnatwf,
      gnatwg,
      gnatwh,
      gnatwi,
      gnatwj,
      gnatwk,
      gnatwl,
      gnatwm,
      gnatwo,
      gnatwp,
      gnatwq,
      gnatwr,
      gnatwt,
      gnatwu,
      gnatwv,
      gnatww,
      gnatwx,
      gnatwy,
      gnatwz,
      gnatw_dot_a,
      gnatw_dot_b,
      gnatw_dot_c,
      gnatw_dot_f,
      gnatw_dot_h,
      gnatw_dot_i,
      gnatw_dot_j,
      gnatw_dot_k,
      gnatw_dot_l,
      gnatw_dot_m,
      gnatw_dot_n,
      gnatw_dot_o,
      gnatw_dot_p,
      gnatw_dot_q,
      gnatw_dot_r,
      gnatw_dot_s,
      gnatw_dot_t,
      gnatw_dot_u,
      gnatw_dot_v,
      gnatw_dot_w,
      gnatw_dot_x,
      gnatw_dot_y,
      gnatw_dot_z,
      gnatw_underscore_a,
      gnatw_underscore_c,
      gnatw_underscore_j,
      gnatw_underscore_l,
      gnatw_underscore_p,
      gnatw_underscore_q,
      gnatw_underscore_r,
      gnatw_underscore_s,
      gnaty,
      gnatya,
      gnatyb,
      gnatyc,
      gnatyd,
      gnatye,
      gnatyf,
      gnatyh,
      gnatyi,
      gnatyk,
      gnatyl,
      gnatym,
      gnatyn,
      gnatyo,
      gnatyp,
      gnatyr,
      gnatys,
      gnatyu,
      gnatyx,
      gnatyz,
      gnatyaa,
      gnatybb,
      gnatycc,
      gnatydd,
      gnatyii,
      gnatyll,
      gnatymm,
      gnatyoo,
      gnatyss,
      gnatytt
   );

   subtype Active_Switch_Id is Switch_Id range gnatel .. gnatytt;

   function Get_Documentation_File (Id : Diagnostic_Id) return String;
   --  Return the location of the documentation file as a string.

   function To_String (Id : Diagnostic_Id) return String;
   --  Convert the diagnostic ID to a 4 character string padded with 0-s.

   procedure Print_Diagnostic_Repository;
   --  Print all of the Diagnostic_Id-s and Switch_Id-s as rules in the SARIF
   --  format.

end Errid;
