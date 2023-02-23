/* function_base declaration for RISC-V 'V' Extension for GNU compiler.
   Copyright (C) 2022-2023 Free Software Foundation, Inc.
   Contributed by Ju-Zhe Zhong (juzhe.zhong@rivai.ai), RiVAI Technologies Ltd.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#ifndef GCC_RISCV_VECTOR_BUILTINS_BASES_H
#define GCC_RISCV_VECTOR_BUILTINS_BASES_H

namespace riscv_vector {

namespace bases {
extern const function_base *const vsetvl;
extern const function_base *const vsetvlmax;
extern const function_base *const vle;
extern const function_base *const vse;
extern const function_base *const vlm;
extern const function_base *const vsm;
extern const function_base *const vlse;
extern const function_base *const vsse;
extern const function_base *const vluxei8;
extern const function_base *const vluxei16;
extern const function_base *const vluxei32;
extern const function_base *const vluxei64;
extern const function_base *const vloxei8;
extern const function_base *const vloxei16;
extern const function_base *const vloxei32;
extern const function_base *const vloxei64;
extern const function_base *const vsuxei8;
extern const function_base *const vsuxei16;
extern const function_base *const vsuxei32;
extern const function_base *const vsuxei64;
extern const function_base *const vsoxei8;
extern const function_base *const vsoxei16;
extern const function_base *const vsoxei32;
extern const function_base *const vsoxei64;
extern const function_base *const vadd;
extern const function_base *const vsub;
extern const function_base *const vrsub;
extern const function_base *const vand;
extern const function_base *const vor;
extern const function_base *const vxor;
extern const function_base *const vsll;
extern const function_base *const vsra;
extern const function_base *const vsrl;
extern const function_base *const vmin;
extern const function_base *const vmax;
extern const function_base *const vminu;
extern const function_base *const vmaxu;
extern const function_base *const vmul;
extern const function_base *const vmulh;
extern const function_base *const vmulhu;
extern const function_base *const vmulhsu;
extern const function_base *const vdiv;
extern const function_base *const vrem;
extern const function_base *const vdivu;
extern const function_base *const vremu;
extern const function_base *const vneg;
extern const function_base *const vnot;
extern const function_base *const vsext;
extern const function_base *const vzext;
extern const function_base *const vwadd;
extern const function_base *const vwsub;
extern const function_base *const vwmul;
extern const function_base *const vwaddu;
extern const function_base *const vwsubu;
extern const function_base *const vwmulu;
extern const function_base *const vwmulsu;
extern const function_base *const vwcvt_x;
extern const function_base *const vwcvtu_x;
extern const function_base *const vadc;
extern const function_base *const vsbc;
extern const function_base *const vmadc;
extern const function_base *const vmsbc;
extern const function_base *const vnsrl;
extern const function_base *const vnsra;
extern const function_base *const vncvt_x;
extern const function_base *const vmerge;
extern const function_base *const vmv_v;
extern const function_base *const vmseq;
extern const function_base *const vmsne;
extern const function_base *const vmslt;
extern const function_base *const vmsgt;
extern const function_base *const vmsle;
extern const function_base *const vmsge;
extern const function_base *const vmsltu;
extern const function_base *const vmsgtu;
extern const function_base *const vmsleu;
extern const function_base *const vmsgeu;
extern const function_base *const vmacc;
extern const function_base *const vnmsac;
extern const function_base *const vmadd;
extern const function_base *const vnmsub;
extern const function_base *const vwmacc;
extern const function_base *const vwmaccu;
extern const function_base *const vwmaccsu;
extern const function_base *const vwmaccus;
extern const function_base *const vsadd;
extern const function_base *const vssub;
extern const function_base *const vsaddu;
extern const function_base *const vssubu;
extern const function_base *const vaadd;
extern const function_base *const vasub;
extern const function_base *const vaaddu;
extern const function_base *const vasubu;
extern const function_base *const vsmul;
extern const function_base *const vssra;
extern const function_base *const vssrl;
extern const function_base *const vnclip;
extern const function_base *const vnclip;
extern const function_base *const vnclipu;
extern const function_base *const vnclipu;
extern const function_base *const vmand;
extern const function_base *const vmnand;
extern const function_base *const vmandn;
extern const function_base *const vmxor;
extern const function_base *const vmor;
extern const function_base *const vmnor;
extern const function_base *const vmorn;
extern const function_base *const vmxnor;
extern const function_base *const vmmv;
extern const function_base *const vmclr;
extern const function_base *const vmset;
extern const function_base *const vmnot;
extern const function_base *const vcpop;
extern const function_base *const vfirst;
extern const function_base *const vmsbf;
extern const function_base *const vmsif;
extern const function_base *const vmsof;
extern const function_base *const viota;
extern const function_base *const vid;
}

} // end namespace riscv_vector

#endif
