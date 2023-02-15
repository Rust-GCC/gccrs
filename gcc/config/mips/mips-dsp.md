;; Copyright (C) 2005-2023 Free Software Foundation, Inc.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.
;;
;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; MIPS DSP ASE Revision 0.98 3/24/2005
(define_c_enum "unspec" [
  UNSPEC_ADDQ
  UNSPEC_ADDQ_S
  UNSPEC_SUBQ
  UNSPEC_SUBQ_S
  UNSPEC_ADDSC
  UNSPEC_ADDWC
  UNSPEC_MODSUB
  UNSPEC_RADDU_W_QB
  UNSPEC_ABSQ_S
  UNSPEC_PRECRQ_QB_PH
  UNSPEC_PRECRQ_PH_W
  UNSPEC_PRECRQ_RS_PH_W
  UNSPEC_PRECRQU_S_QB_PH
  UNSPEC_PRECEQ_W_PHL
  UNSPEC_PRECEQ_W_PHR
  UNSPEC_PRECEQU_PH_QBL
  UNSPEC_PRECEQU_PH_QBR
  UNSPEC_PRECEQU_PH_QBLA
  UNSPEC_PRECEQU_PH_QBRA
  UNSPEC_PRECEU_PH_QBL
  UNSPEC_PRECEU_PH_QBR
  UNSPEC_PRECEU_PH_QBLA
  UNSPEC_PRECEU_PH_QBRA
  UNSPEC_SHLL
  UNSPEC_SHLL_S
  UNSPEC_SHRL_QB
  UNSPEC_SHRA_PH
  UNSPEC_SHRA_R
  UNSPEC_MULEU_S_PH_QBL
  UNSPEC_MULEU_S_PH_QBR
  UNSPEC_MULQ_RS_PH
  UNSPEC_MULEQ_S_W_PHL
  UNSPEC_MULEQ_S_W_PHR
  UNSPEC_DPAU_H_QBL
  UNSPEC_DPAU_H_QBR
  UNSPEC_DPSU_H_QBL
  UNSPEC_DPSU_H_QBR
  UNSPEC_DPAQ_S_W_PH
  UNSPEC_DPSQ_S_W_PH
  UNSPEC_MULSAQ_S_W_PH
  UNSPEC_DPAQ_SA_L_W
  UNSPEC_DPSQ_SA_L_W
  UNSPEC_MAQ_S_W_PHL
  UNSPEC_MAQ_S_W_PHR
  UNSPEC_MAQ_SA_W_PHL
  UNSPEC_MAQ_SA_W_PHR
  UNSPEC_BITREV
  UNSPEC_INSV
  UNSPEC_REPL_QB
  UNSPEC_REPL_PH
  UNSPEC_CMP_EQ
  UNSPEC_CMP_LT
  UNSPEC_CMP_LE
  UNSPEC_CMPGU_EQ_QB
  UNSPEC_CMPGU_LT_QB
  UNSPEC_CMPGU_LE_QB
  UNSPEC_PICK
  UNSPEC_PACKRL_PH
  UNSPEC_EXTR_W
  UNSPEC_EXTR_R_W
  UNSPEC_EXTR_RS_W
  UNSPEC_EXTR_S_H
  UNSPEC_EXTP
  UNSPEC_EXTPDP
  UNSPEC_SHILO
  UNSPEC_MTHLIP
  UNSPEC_WRDSP
  UNSPEC_RDDSP
])

(define_constants
  [(CCDSP_PO_REGNUM	182)
   (CCDSP_SC_REGNUM	183)
   (CCDSP_CA_REGNUM	184)
   (CCDSP_OU_REGNUM	185)
   (CCDSP_CC_REGNUM	186)
   (CCDSP_EF_REGNUM	187)])

;; This mode iterator allows si, v2hi, v4qi for all possible modes in DSP ASE.
(define_mode_iterator DSP [(SI "ISA_HAS_DSP")
			   (V2HI "ISA_HAS_DSP")
		 	   (V4QI "ISA_HAS_DSP")])

;; This mode iterator allows v2hi, v4qi for vector/SIMD data.
(define_mode_iterator DSPV [(V2HI "ISA_HAS_DSP")
			    (V4QI "ISA_HAS_DSP")])

;; This mode iterator allows si, v2hi for Q31 and V2Q15 fixed-point data.
(define_mode_iterator DSPQ [(SI "ISA_HAS_DSP")
			    (V2HI "ISA_HAS_DSP")])

;; DSP instructions use q for fixed-point data, and u for integer in the infix.
(define_mode_attr dspfmt1 [(SI "q") (V2HI "q") (V4QI "u")])

;; DSP instructions use nothing for fixed-point data, and u for integer in
;; the infix.
(define_mode_attr dspfmt1_1 [(SI "") (V2HI "") (V4QI "u")])

;; DSP instructions use w, ph, qb in the postfix.
(define_mode_attr dspfmt2 [(SI "w") (V2HI "ph") (V4QI "qb")])

;; DSP shift masks for SI, V2HI, V4QI.
(define_mode_attr dspshift_mask [(SI "0x1f") (V2HI "0xf") (V4QI "0x7")])

;; MIPS DSP ASE Revision 0.98 3/24/2005
;; Table 2-1. MIPS DSP ASE Instructions: Arithmetic
;; ADDQ*
(define_insn "add<DSPV:mode>3"
  [(set (match_operand:DSPV 0 "register_operand" "=d")
	(plus:DSPV (match_operand:DSPV 1 "register_operand" "d")
		   (match_operand:DSPV 2 "register_operand" "d")))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_ADDQ))]
  "ISA_HAS_DSP"
  "add<DSPV:dspfmt1>.<DSPV:dspfmt2>\t%0,%1,%2"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_add<DSP:dspfmt1>_s_<DSP:dspfmt2>"
  [(set (match_operand:DSP 0 "register_operand" "=d")
	(unspec:DSP [(match_operand:DSP 1 "register_operand" "d")
		     (match_operand:DSP 2 "register_operand" "d")]
		    UNSPEC_ADDQ_S))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_ADDQ_S))]
  "ISA_HAS_DSP"
  "add<DSP:dspfmt1>_s.<DSP:dspfmt2>\t%0,%1,%2"
  [(set_attr "type"	"dspalusat")
   (set_attr "mode"	"SI")])

;; SUBQ*
(define_insn "sub<DSPV:mode>3"
  [(set (match_operand:DSPV 0 "register_operand" "=d")
	(minus:DSPV (match_operand:DSPV 1 "register_operand" "d")
		    (match_operand:DSPV 2 "register_operand" "d")))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_SUBQ))]
  "ISA_HAS_DSP"
  "sub<DSPV:dspfmt1>.<DSPV:dspfmt2>\t%0,%1,%2"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_sub<DSP:dspfmt1>_s_<DSP:dspfmt2>"
  [(set (match_operand:DSP 0 "register_operand" "=d")
	(unspec:DSP [(match_operand:DSP 1 "register_operand" "d")
		     (match_operand:DSP 2 "register_operand" "d")]
		    UNSPEC_SUBQ_S))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_SUBQ_S))]
  "ISA_HAS_DSP"
  "sub<DSP:dspfmt1>_s.<DSP:dspfmt2>\t%0,%1,%2"
  [(set_attr "type"	"dspalusat")
   (set_attr "mode"	"SI")])

;; ADDSC
(define_insn "mips_addsc"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:SI 1 "register_operand" "d")
		    (match_operand:SI 2 "register_operand" "d")]
		   UNSPEC_ADDSC))
   (set (reg:CCDSP CCDSP_CA_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_ADDSC))]
  "ISA_HAS_DSP"
  "addsc\t%0,%1,%2"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; ADDWC
(define_insn "mips_addwc"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:SI 1 "register_operand" "d")
		    (match_operand:SI 2 "register_operand" "d")
		  (reg:CCDSP CCDSP_CA_REGNUM)]
		   UNSPEC_ADDWC))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_ADDWC))]
  "ISA_HAS_DSP"
  "addwc\t%0,%1,%2"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; MODSUB
(define_insn "mips_modsub"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:SI 1 "register_operand" "d")
		    (match_operand:SI 2 "register_operand" "d")]
		   UNSPEC_MODSUB))]
  "ISA_HAS_DSP"
  "modsub\t%0,%1,%2"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; RADDU*
(define_insn "mips_raddu_w_qb"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:V4QI 1 "register_operand" "d")]
		   UNSPEC_RADDU_W_QB))]
  "ISA_HAS_DSP"
  "raddu.w.qb\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; ABSQ*
(define_insn "mips_absq_s_<DSPQ:dspfmt2>"
  [(set (match_operand:DSPQ 0 "register_operand" "=d")
	(unspec:DSPQ [(match_operand:DSPQ 1 "register_operand" "d")]
		     UNSPEC_ABSQ_S))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1)] UNSPEC_ABSQ_S))]
  "ISA_HAS_DSP"
  "absq_s.<DSPQ:dspfmt2>\t%0,%1"
  [(set_attr "type"	"dspalusat")
   (set_attr "mode"	"SI")])

;; PRECRQ*
(define_insn "mips_precrq_qb_ph"
  [(set (match_operand:V4QI 0 "register_operand" "=d")
	(unspec:V4QI [(match_operand:V2HI 1 "register_operand" "d")
		      (match_operand:V2HI 2 "register_operand" "d")]
		     UNSPEC_PRECRQ_QB_PH))]
  "ISA_HAS_DSP"
  "precrq.qb.ph\t%0,%1,%2"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_precrq_ph_w"
  [(set (match_operand:V2HI 0 "register_operand" "=d")
	(unspec:V2HI [(match_operand:SI 1 "register_operand" "d")
		      (match_operand:SI 2 "register_operand" "d")]
		     UNSPEC_PRECRQ_PH_W))]
  "ISA_HAS_DSP"
  "precrq.ph.w\t%0,%1,%2"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_precrq_rs_ph_w"
  [(set (match_operand:V2HI 0 "register_operand" "=d")
	(unspec:V2HI [(match_operand:SI 1 "register_operand" "d")
		      (match_operand:SI 2 "register_operand" "d")]
		     UNSPEC_PRECRQ_RS_PH_W))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)]
		      UNSPEC_PRECRQ_RS_PH_W))]
  "ISA_HAS_DSP"
  "precrq_rs.ph.w\t%0,%1,%2"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; PRECRQU*
(define_insn "mips_precrqu_s_qb_ph"
  [(set (match_operand:V4QI 0 "register_operand" "=d")
	(unspec:V4QI [(match_operand:V2HI 1 "register_operand" "d")
		      (match_operand:V2HI 2 "register_operand" "d")]
		     UNSPEC_PRECRQU_S_QB_PH))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)]
		      UNSPEC_PRECRQU_S_QB_PH))]
  "ISA_HAS_DSP"
  "precrqu_s.qb.ph\t%0,%1,%2"
  [(set_attr "type"	"dspalusat")
   (set_attr "mode"	"SI")])

;; PRECEQ*
(define_insn "mips_preceq_w_phl"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:V2HI 1 "register_operand" "d")]
		   UNSPEC_PRECEQ_W_PHL))]
  "ISA_HAS_DSP"
  "preceq.w.phl\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_preceq_w_phr"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:V2HI 1 "register_operand" "d")]
		   UNSPEC_PRECEQ_W_PHR))]
  "ISA_HAS_DSP"
  "preceq.w.phr\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; PRECEQU*
(define_insn "mips_precequ_ph_qbl"
  [(set (match_operand:V2HI 0 "register_operand" "=d")
	(unspec:V2HI [(match_operand:V4QI 1 "register_operand" "d")]
		     UNSPEC_PRECEQU_PH_QBL))]
  "ISA_HAS_DSP"
  "precequ.ph.qbl\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_precequ_ph_qbr"
  [(set (match_operand:V2HI 0 "register_operand" "=d")
	(unspec:V2HI [(match_operand:V4QI 1 "register_operand" "d")]
		     UNSPEC_PRECEQU_PH_QBR))]
  "ISA_HAS_DSP"
  "precequ.ph.qbr\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_precequ_ph_qbla"
  [(set (match_operand:V2HI 0 "register_operand" "=d")
	(unspec:V2HI [(match_operand:V4QI 1 "register_operand" "d")]
		     UNSPEC_PRECEQU_PH_QBLA))]
  "ISA_HAS_DSP"
  "precequ.ph.qbla\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_precequ_ph_qbra"
  [(set (match_operand:V2HI 0 "register_operand" "=d")
	(unspec:V2HI [(match_operand:V4QI 1 "register_operand" "d")]
		     UNSPEC_PRECEQU_PH_QBRA))]
  "ISA_HAS_DSP"
  "precequ.ph.qbra\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; PRECEU*
(define_insn "mips_preceu_ph_qbl"
  [(set (match_operand:V2HI 0 "register_operand" "=d")
	(unspec:V2HI [(match_operand:V4QI 1 "register_operand" "d")]
		     UNSPEC_PRECEU_PH_QBL))]
  "ISA_HAS_DSP"
  "preceu.ph.qbl\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_preceu_ph_qbr"
  [(set (match_operand:V2HI 0 "register_operand" "=d")
	(unspec:V2HI [(match_operand:V4QI 1 "register_operand" "d")]
		     UNSPEC_PRECEU_PH_QBR))]
  "ISA_HAS_DSP"
  "preceu.ph.qbr\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_preceu_ph_qbla"
  [(set (match_operand:V2HI 0 "register_operand" "=d")
	(unspec:V2HI [(match_operand:V4QI 1 "register_operand" "d")]
		     UNSPEC_PRECEU_PH_QBLA))]
  "ISA_HAS_DSP"
  "preceu.ph.qbla\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_preceu_ph_qbra"
  [(set (match_operand:V2HI 0 "register_operand" "=d")
	(unspec:V2HI [(match_operand:V4QI 1 "register_operand" "d")]
		     UNSPEC_PRECEU_PH_QBRA))]
  "ISA_HAS_DSP"
  "preceu.ph.qbra\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; Table 2-2. MIPS DSP ASE Instructions: Shift
;; SHLL*
(define_insn "mips_shll_<DSPV:dspfmt2>"
  [(set (match_operand:DSPV 0 "register_operand" "=d,d")
	(unspec:DSPV [(match_operand:DSPV 1 "register_operand" "d,d")
		      (match_operand:SI 2 "arith_operand" "I,d")]
		     UNSPEC_SHLL))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_SHLL))]
  "ISA_HAS_DSP"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2])
	  & ~(unsigned HOST_WIDE_INT) <DSPV:dspshift_mask>)
	operands[2] = GEN_INT (INTVAL (operands[2]) & <DSPV:dspshift_mask>);
      return "shll.<DSPV:dspfmt2>\t%0,%1,%2";
    }
  return "shllv.<DSPV:dspfmt2>\t%0,%1,%2";
}
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_shll_s_<DSPQ:dspfmt2>"
  [(set (match_operand:DSPQ 0 "register_operand" "=d,d")
	(unspec:DSPQ [(match_operand:DSPQ 1 "register_operand" "d,d")
		      (match_operand:SI 2 "arith_operand" "I,d")]
		     UNSPEC_SHLL_S))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_SHLL_S))]
  "ISA_HAS_DSP"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2])
          & ~(unsigned HOST_WIDE_INT) <DSPQ:dspshift_mask>)
	operands[2] = GEN_INT (INTVAL (operands[2]) & <DSPQ:dspshift_mask>);
      return "shll_s.<DSPQ:dspfmt2>\t%0,%1,%2";
    }
  return "shllv_s.<DSPQ:dspfmt2>\t%0,%1,%2";
}
  [(set_attr "type"	"dspalusat")
   (set_attr "mode"	"SI")])

;; SHRL*
(define_insn "mips_shrl_qb"
  [(set (match_operand:V4QI 0 "register_operand" "=d,d")
	(unspec:V4QI [(match_operand:V4QI 1 "register_operand" "d,d")
		      (match_operand:SI 2 "arith_operand" "I,d")]
		     UNSPEC_SHRL_QB))]
  "ISA_HAS_DSP"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0x7)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x7);
      return "shrl.qb\t%0,%1,%2";
    }
  return "shrlv.qb\t%0,%1,%2";
}
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; SHRA*
(define_insn "mips_shra_ph"
  [(set (match_operand:V2HI 0 "register_operand" "=d,d")
	(unspec:V2HI [(match_operand:V2HI 1 "register_operand" "d,d")
		      (match_operand:SI 2 "arith_operand" "I,d")]
		     UNSPEC_SHRA_PH))]
  "ISA_HAS_DSP"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0xf)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0xf);
      return "shra.ph\t%0,%1,%2";
    }
  return "shrav.ph\t%0,%1,%2";
}
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_shra_r_<DSPQ:dspfmt2>"
  [(set (match_operand:DSPQ 0 "register_operand" "=d,d")
	(unspec:DSPQ [(match_operand:DSPQ 1 "register_operand" "d,d")
		      (match_operand:SI 2 "arith_operand" "I,d")]
		     UNSPEC_SHRA_R))]
  "ISA_HAS_DSP"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2])
	  & ~(unsigned HOST_WIDE_INT) <DSPQ:dspshift_mask>)
	operands[2] = GEN_INT (INTVAL (operands[2]) & <DSPQ:dspshift_mask>);
      return "shra_r.<DSPQ:dspfmt2>\t%0,%1,%2";
    }
  return "shrav_r.<DSPQ:dspfmt2>\t%0,%1,%2";
}
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; Table 2-3. MIPS DSP ASE Instructions: Multiply
;; MULEU*
(define_insn "mips_muleu_s_ph_qbl"
  [(set (match_operand:V2HI 0 "register_operand" "=d")
	(unspec:V2HI [(match_operand:V4QI 1 "register_operand" "d")
		      (match_operand:V2HI 2 "register_operand" "d")]
		     UNSPEC_MULEU_S_PH_QBL))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_MULEU_S_PH_QBL))
   (clobber (match_scratch:DI 3 "=x"))]
  "ISA_HAS_DSP"
  "muleu_s.ph.qbl\t%0,%1,%2"
  [(set_attr "type"	"imul3")
   (set_attr "mode"	"SI")])

(define_insn "mips_muleu_s_ph_qbr"
  [(set (match_operand:V2HI 0 "register_operand" "=d")
	(unspec:V2HI [(match_operand:V4QI 1 "register_operand" "d")
		      (match_operand:V2HI 2 "register_operand" "d")]
		     UNSPEC_MULEU_S_PH_QBR))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_MULEU_S_PH_QBR))
   (clobber (match_scratch:DI 3 "=x"))]
  "ISA_HAS_DSP"
  "muleu_s.ph.qbr\t%0,%1,%2"
  [(set_attr "type"	"imul3")
   (set_attr "mode"	"SI")])

;; MULQ*
(define_insn "mips_mulq_rs_ph"
  [(set (match_operand:V2HI 0 "register_operand" "=d")
	(unspec:V2HI [(match_operand:V2HI 1 "register_operand" "d")
		      (match_operand:V2HI 2 "register_operand" "d")]
		     UNSPEC_MULQ_RS_PH))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_MULQ_RS_PH))
   (clobber (match_scratch:DI 3 "=x"))]
  "ISA_HAS_DSP"
  "mulq_rs.ph\t%0,%1,%2"
  [(set_attr "type"	"imul3")
   (set_attr "mode"	"SI")])

;; MULEQ*
(define_insn "mips_muleq_s_w_phl"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:V2HI 1 "register_operand" "d")
		    (match_operand:V2HI 2 "register_operand" "d")]
		   UNSPEC_MULEQ_S_W_PHL))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_MULEQ_S_W_PHL))
   (clobber (match_scratch:DI 3 "=x"))]
  "ISA_HAS_DSP"
  "muleq_s.w.phl\t%0,%1,%2"
  [(set_attr "type"	"imul3")
   (set_attr "mode"	"SI")])

(define_insn "mips_muleq_s_w_phr"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:V2HI 1 "register_operand" "d")
		    (match_operand:V2HI 2 "register_operand" "d")]
		   UNSPEC_MULEQ_S_W_PHR))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_MULEQ_S_W_PHR))
   (clobber (match_scratch:DI 3 "=x"))]
  "ISA_HAS_DSP"
  "muleq_s.w.phr\t%0,%1,%2"
  [(set_attr "type"	"imul3")
   (set_attr "mode"	"SI")])

;; DPAU*
(define_insn "mips_dpau_h_qbl"
  [(set (match_operand:DI 0 "register_operand" "=a")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:V4QI 2 "register_operand" "d")
		    (match_operand:V4QI 3 "register_operand" "d")]
		   UNSPEC_DPAU_H_QBL))]
  "ISA_HAS_DSP && !TARGET_64BIT"
  "dpau.h.qbl\t%q0,%2,%3"
  [(set_attr "type"	"dspmac")
   (set_attr "accum_in" "1")
   (set_attr "mode"	"SI")])

(define_insn "mips_dpau_h_qbr"
  [(set (match_operand:DI 0 "register_operand" "=a")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:V4QI 2 "register_operand" "d")
		    (match_operand:V4QI 3 "register_operand" "d")]
		   UNSPEC_DPAU_H_QBR))]
  "ISA_HAS_DSP && !TARGET_64BIT"
  "dpau.h.qbr\t%q0,%2,%3"
  [(set_attr "type"	"dspmac")
   (set_attr "accum_in" "1")
   (set_attr "mode"	"SI")])

;; DPSU*
(define_insn "mips_dpsu_h_qbl"
  [(set (match_operand:DI 0 "register_operand" "=a")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:V4QI 2 "register_operand" "d")
		    (match_operand:V4QI 3 "register_operand" "d")]
		   UNSPEC_DPSU_H_QBL))]
  "ISA_HAS_DSP && !TARGET_64BIT"
  "dpsu.h.qbl\t%q0,%2,%3"
  [(set_attr "type"	"dspmac")
   (set_attr "accum_in" "1")
   (set_attr "mode"	"SI")])

(define_insn "mips_dpsu_h_qbr"
  [(set (match_operand:DI 0 "register_operand" "=a")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:V4QI 2 "register_operand" "d")
		    (match_operand:V4QI 3 "register_operand" "d")]
		   UNSPEC_DPSU_H_QBR))]
  "ISA_HAS_DSP && !TARGET_64BIT"
  "dpsu.h.qbr\t%q0,%2,%3"
  [(set_attr "type"	"dspmac")
   (set_attr "accum_in" "1")
   (set_attr "mode"	"SI")])

;; DPAQ*
(define_insn "mips_dpaq_s_w_ph"
  [(set (match_operand:DI 0 "register_operand" "=a")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:V2HI 2 "register_operand" "d")
		    (match_operand:V2HI 3 "register_operand" "d")]
		   UNSPEC_DPAQ_S_W_PH))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2) (match_dup 3)]
		      UNSPEC_DPAQ_S_W_PH))]
  "ISA_HAS_DSP && !TARGET_64BIT"
  "dpaq_s.w.ph\t%q0,%2,%3"
  [(set_attr "type"	"dspmac")
   (set_attr "accum_in" "1")
   (set_attr "mode"	"SI")])

;; DPSQ*
(define_insn "mips_dpsq_s_w_ph"
  [(set (match_operand:DI 0 "register_operand" "=a")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:V2HI 2 "register_operand" "d")
		    (match_operand:V2HI 3 "register_operand" "d")]
		   UNSPEC_DPSQ_S_W_PH))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2) (match_dup 3)]
		      UNSPEC_DPSQ_S_W_PH))]
  "ISA_HAS_DSP && !TARGET_64BIT"
  "dpsq_s.w.ph\t%q0,%2,%3"
  [(set_attr "type"	"dspmac")
   (set_attr "accum_in" "1")
   (set_attr "mode"	"SI")])

;; MULSAQ*
(define_insn "mips_mulsaq_s_w_ph"
  [(set (match_operand:DI 0 "register_operand" "=a")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:V2HI 2 "register_operand" "d")
		    (match_operand:V2HI 3 "register_operand" "d")]
		   UNSPEC_MULSAQ_S_W_PH))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2) (match_dup 3)]
		      UNSPEC_MULSAQ_S_W_PH))]
  "ISA_HAS_DSP && !TARGET_64BIT"
  "mulsaq_s.w.ph\t%q0,%2,%3"
  [(set_attr "type"	"dspmac")
   (set_attr "accum_in" "1")
   (set_attr "mode"	"SI")])

;; DPAQ*
(define_insn "mips_dpaq_sa_l_w"
  [(set (match_operand:DI 0 "register_operand" "=a")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:SI 2 "register_operand" "d")
		    (match_operand:SI 3 "register_operand" "d")]
		   UNSPEC_DPAQ_SA_L_W))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2) (match_dup 3)]
		      UNSPEC_DPAQ_SA_L_W))]
  "ISA_HAS_DSP && !TARGET_64BIT"
  "dpaq_sa.l.w\t%q0,%2,%3"
  [(set_attr "type"	"dspmacsat")
   (set_attr "accum_in" "1")
   (set_attr "mode"	"SI")])

;; DPSQ*
(define_insn "mips_dpsq_sa_l_w"
  [(set (match_operand:DI 0 "register_operand" "=a")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:SI 2 "register_operand" "d")
		    (match_operand:SI 3 "register_operand" "d")]
		   UNSPEC_DPSQ_SA_L_W))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2) (match_dup 3)]
		      UNSPEC_DPSQ_SA_L_W))]
  "ISA_HAS_DSP && !TARGET_64BIT"
  "dpsq_sa.l.w\t%q0,%2,%3"
  [(set_attr "type"	"dspmacsat")
   (set_attr "accum_in" "1")
   (set_attr "mode"	"SI")])

;; MAQ*
(define_insn "mips_maq_s_w_phl"
  [(set (match_operand:DI 0 "register_operand" "=a")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:V2HI 2 "register_operand" "d")
		    (match_operand:V2HI 3 "register_operand" "d")]
		   UNSPEC_MAQ_S_W_PHL))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2) (match_dup 3)]
		      UNSPEC_MAQ_S_W_PHL))]
  "ISA_HAS_DSP && !TARGET_64BIT"
  "maq_s.w.phl\t%q0,%2,%3"
  [(set_attr "type"	"dspmac")
   (set_attr "accum_in" "1")
   (set_attr "mode"	"SI")])

(define_insn "mips_maq_s_w_phr"
  [(set (match_operand:DI 0 "register_operand" "=a")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:V2HI 2 "register_operand" "d")
		    (match_operand:V2HI 3 "register_operand" "d")]
		   UNSPEC_MAQ_S_W_PHR))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2) (match_dup 3)]
		      UNSPEC_MAQ_S_W_PHR))]
  "ISA_HAS_DSP && !TARGET_64BIT"
  "maq_s.w.phr\t%q0,%2,%3"
  [(set_attr "type"	"dspmac")
   (set_attr "accum_in" "1")
   (set_attr "mode"	"SI")])

;; MAQ_SA*
(define_insn "mips_maq_sa_w_phl"
  [(set (match_operand:DI 0 "register_operand" "=a")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:V2HI 2 "register_operand" "d")
		    (match_operand:V2HI 3 "register_operand" "d")]
		   UNSPEC_MAQ_SA_W_PHL))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2) (match_dup 3)]
		      UNSPEC_MAQ_SA_W_PHL))]
  "ISA_HAS_DSP && !TARGET_64BIT"
  "maq_sa.w.phl\t%q0,%2,%3"
  [(set_attr "type"	"dspmacsat")
   (set_attr "accum_in" "1")
   (set_attr "mode"	"SI")])

(define_insn "mips_maq_sa_w_phr"
  [(set (match_operand:DI 0 "register_operand" "=a")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:V2HI 2 "register_operand" "d")
		    (match_operand:V2HI 3 "register_operand" "d")]
		   UNSPEC_MAQ_SA_W_PHR))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2) (match_dup 3)]
		      UNSPEC_MAQ_SA_W_PHR))]
  "ISA_HAS_DSP && !TARGET_64BIT"
  "maq_sa.w.phr\t%q0,%2,%3"
  [(set_attr "type"	"dspmacsat")
   (set_attr "accum_in" "1")
   (set_attr "mode"	"SI")])

;; Table 2-4. MIPS DSP ASE Instructions: General Bit/Manipulation
;; BITREV
(define_insn "mips_bitrev"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:SI 1 "register_operand" "d")]
		   UNSPEC_BITREV))]
  "ISA_HAS_DSP"
  "bitrev\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; INSV
(define_insn "mips_insv"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:SI 1 "register_operand" "0")
		    (match_operand:SI 2 "register_operand" "d")
		    (reg:CCDSP CCDSP_SC_REGNUM)
		    (reg:CCDSP CCDSP_PO_REGNUM)]
		   UNSPEC_INSV))]
  "ISA_HAS_DSP"
  "insv\t%0,%2"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; REPL*
(define_insn "mips_repl_qb"
  [(set (match_operand:V4QI 0 "register_operand" "=d,d")
	(unspec:V4QI [(match_operand:SI 1 "arith_operand" "I,d")]
		     UNSPEC_REPL_QB))]
  "ISA_HAS_DSP"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[1]) & ~(unsigned HOST_WIDE_INT) 0xff)
	operands[1] = GEN_INT (INTVAL (operands[1]) & 0xff);
      return "repl.qb\t%0,%1";
    }
  return "replv.qb\t%0,%1";
}
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_repl_ph"
  [(set (match_operand:V2HI 0 "register_operand" "=d,d")
	(unspec:V2HI [(match_operand:SI 1 "reg_imm10_operand" "YB,d")]
		     UNSPEC_REPL_PH))]
  "ISA_HAS_DSP"
  "@
   repl.ph\t%0,%1
   replv.ph\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; Table 2-5. MIPS DSP ASE Instructions: Compare-Pick
;; CMPU.* CMP.*
(define_insn "mips_cmp<DSPV:dspfmt1_1>_eq_<DSPV:dspfmt2>"
  [(set (reg:CCDSP CCDSP_CC_REGNUM)
	(unspec:CCDSP [(match_operand:DSPV 0 "register_operand" "d")
		       (match_operand:DSPV 1 "register_operand" "d")
		       (reg:CCDSP CCDSP_CC_REGNUM)]
		      UNSPEC_CMP_EQ))]
  "ISA_HAS_DSP"
  "cmp<DSPV:dspfmt1_1>.eq.<DSPV:dspfmt2>\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_cmp<DSPV:dspfmt1_1>_lt_<DSPV:dspfmt2>"
  [(set (reg:CCDSP CCDSP_CC_REGNUM)
	(unspec:CCDSP [(match_operand:DSPV 0 "register_operand" "d")
		       (match_operand:DSPV 1 "register_operand" "d")
		       (reg:CCDSP CCDSP_CC_REGNUM)]
		      UNSPEC_CMP_LT))]
  "ISA_HAS_DSP"
  "cmp<DSPV:dspfmt1_1>.lt.<DSPV:dspfmt2>\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_cmp<DSPV:dspfmt1_1>_le_<DSPV:dspfmt2>"
  [(set (reg:CCDSP CCDSP_CC_REGNUM)
	(unspec:CCDSP [(match_operand:DSPV 0 "register_operand" "d")
		       (match_operand:DSPV 1 "register_operand" "d")
		       (reg:CCDSP CCDSP_CC_REGNUM)]
		      UNSPEC_CMP_LE))]
  "ISA_HAS_DSP"
  "cmp<DSPV:dspfmt1_1>.le.<DSPV:dspfmt2>\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_cmpgu_eq_qb"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:V4QI 1 "register_operand" "d")
		    (match_operand:V4QI 2 "register_operand" "d")]
		   UNSPEC_CMPGU_EQ_QB))]
  "ISA_HAS_DSP"
  "cmpgu.eq.qb\t%0,%1,%2"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_cmpgu_lt_qb"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:V4QI 1 "register_operand" "d")
		    (match_operand:V4QI 2 "register_operand" "d")]
		   UNSPEC_CMPGU_LT_QB))]
  "ISA_HAS_DSP"
  "cmpgu.lt.qb\t%0,%1,%2"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

(define_insn "mips_cmpgu_le_qb"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:V4QI 1 "register_operand" "d")
		    (match_operand:V4QI 2 "register_operand" "d")]
		   UNSPEC_CMPGU_LE_QB))]
  "ISA_HAS_DSP"
  "cmpgu.le.qb\t%0,%1,%2"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; PICK*
(define_insn "mips_pick_<DSPV:dspfmt2>"
  [(set (match_operand:DSPV 0 "register_operand" "=d")
	(unspec:DSPV [(match_operand:DSPV 1 "register_operand" "d")
		      (match_operand:DSPV 2 "register_operand" "d")
		      (reg:CCDSP CCDSP_CC_REGNUM)]
		     UNSPEC_PICK))]
  "ISA_HAS_DSP"
  "pick.<DSPV:dspfmt2>\t%0,%1,%2"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; PACKRL*
(define_insn "mips_packrl_ph"
  [(set (match_operand:V2HI 0 "register_operand" "=d")
	(unspec:V2HI [(match_operand:V2HI 1 "register_operand" "d")
		      (match_operand:V2HI 2 "register_operand" "d")]
		     UNSPEC_PACKRL_PH))]
  "ISA_HAS_DSP"
  "packrl.ph\t%0,%1,%2"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; Table 2-6. MIPS DSP ASE Instructions: Accumulator and DSPControl Access
;; EXTR*
(define_insn "mips_extr_w"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(unspec:SI [(match_operand:DI 1 "register_operand" "a,a")
		    (match_operand:SI 2 "arith_operand" "I,d")]
		   UNSPEC_EXTR_W))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_EXTR_W))]
  "ISA_HAS_DSP && !TARGET_64BIT"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0x1f)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
      return "extr.w\t%0,%q1,%2";
    }
  return "extrv.w\t%0,%q1,%2";
}
  [(set_attr "type"	"accext")
   (set_attr "mode"	"SI")])

(define_insn "mips_extr_r_w"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(unspec:SI [(match_operand:DI 1 "register_operand" "a,a")
		    (match_operand:SI 2 "arith_operand" "I,d")]
		   UNSPEC_EXTR_R_W))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_EXTR_R_W))]
  "ISA_HAS_DSP && !TARGET_64BIT"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0x1f)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
      return "extr_r.w\t%0,%q1,%2";
    }
  return "extrv_r.w\t%0,%q1,%2";
}
  [(set_attr "type"	"accext")
   (set_attr "mode"	"SI")])

(define_insn "mips_extr_rs_w"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(unspec:SI [(match_operand:DI 1 "register_operand" "a,a")
		    (match_operand:SI 2 "arith_operand" "I,d")]
		   UNSPEC_EXTR_RS_W))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_EXTR_RS_W))]
  "ISA_HAS_DSP && !TARGET_64BIT"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0x1f)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
      return "extr_rs.w\t%0,%q1,%2";
    }
  return "extrv_rs.w\t%0,%q1,%2";
}
  [(set_attr "type"	"accext")
   (set_attr "mode"	"SI")])

;; EXTR*_S.H
(define_insn "mips_extr_s_h"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(unspec:SI [(match_operand:DI 1 "register_operand" "a,a")
		    (match_operand:SI 2 "arith_operand" "I,d")]
		   UNSPEC_EXTR_S_H))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_EXTR_S_H))]
  "ISA_HAS_DSP && !TARGET_64BIT"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0x1f)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
      return "extr_s.h\t%0,%q1,%2";
    }
  return "extrv_s.h\t%0,%q1,%2";
}
  [(set_attr "type"	"accext")
   (set_attr "mode"	"SI")])

;; EXTP*
(define_insn "mips_extp"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(unspec:SI [(match_operand:DI 1 "register_operand" "a,a")
		    (match_operand:SI 2 "arith_operand" "I,d")
		    (reg:CCDSP CCDSP_PO_REGNUM)]
		   UNSPEC_EXTP))
   (set (reg:CCDSP CCDSP_EF_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_EXTP))]
  "ISA_HAS_DSP && !TARGET_64BIT"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0x1f)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
      return "extp\t%0,%q1,%2";
    }
  return "extpv\t%0,%q1,%2";
}
  [(set_attr "type"	"accext")
   (set_attr "mode"	"SI")])

(define_insn "mips_extpdp"
  [(set (match_operand:SI 0 "register_operand" "=d,d")
	(unspec:SI [(match_operand:DI 1 "register_operand" "a,a")
		    (match_operand:SI 2 "arith_operand" "I,d")
		    (reg:CCDSP CCDSP_PO_REGNUM)]
		   UNSPEC_EXTPDP))
   (set (reg:CCDSP CCDSP_PO_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)
		       (reg:CCDSP CCDSP_PO_REGNUM)] UNSPEC_EXTPDP))
   (set (reg:CCDSP CCDSP_EF_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)] UNSPEC_EXTPDP))]
  "ISA_HAS_DSP && !TARGET_64BIT"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) & ~(unsigned HOST_WIDE_INT) 0x1f)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x1f);
      return "extpdp\t%0,%q1,%2";
    }
  return "extpdpv\t%0,%q1,%2";
}
  [(set_attr "type"	"accext")
   (set_attr "mode"	"SI")])

;; SHILO*
(define_insn "mips_shilo"
  [(set (match_operand:DI 0 "register_operand" "=a,a")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0,0")
		    (match_operand:SI 2 "arith_operand" "I,d")]
		   UNSPEC_SHILO))]
  "ISA_HAS_DSP && !TARGET_64BIT"
{
  if (which_alternative == 0)
    {
      if (INTVAL (operands[2]) < -32 || INTVAL (operands[2]) > 31)
	operands[2] = GEN_INT (INTVAL (operands[2]) & 0x3f);
      return "shilo\t%q0,%2";
    }
  return "shilov\t%q0,%2";
}
  [(set_attr "type"	"accmod")
   (set_attr "mode"	"SI")])

;; MTHLIP*
(define_insn "mips_mthlip"
  [(set (match_operand:DI 0 "register_operand" "=a")
	(unspec:DI [(match_operand:DI 1 "register_operand" "0")
		    (match_operand:SI 2 "register_operand" "d")
		    (reg:CCDSP CCDSP_PO_REGNUM)]
		   UNSPEC_MTHLIP))
   (set (reg:CCDSP CCDSP_PO_REGNUM)
	(unspec:CCDSP [(match_dup 1) (match_dup 2)
		       (reg:CCDSP CCDSP_PO_REGNUM)] UNSPEC_MTHLIP))]
  "ISA_HAS_DSP && !TARGET_64BIT"
  "mthlip\t%2,%q0"
  [(set_attr "type"	"accmod")
   (set_attr "mode"	"SI")])

;; WRDSP
(define_insn "mips_wrdsp"
  [(set (reg:CCDSP CCDSP_PO_REGNUM)
	(unspec:CCDSP [(match_operand:SI 0 "register_operand" "d")
		       (match_operand:SI 1 "const_uimm6_operand" "YA")]
		      UNSPEC_WRDSP))
   (set (reg:CCDSP CCDSP_SC_REGNUM)
	(unspec:CCDSP [(match_dup 0) (match_dup 1)] UNSPEC_WRDSP))
   (set (reg:CCDSP CCDSP_CA_REGNUM)
	(unspec:CCDSP [(match_dup 0) (match_dup 1)] UNSPEC_WRDSP))
   (set (reg:CCDSP CCDSP_OU_REGNUM)
	(unspec:CCDSP [(match_dup 0) (match_dup 1)] UNSPEC_WRDSP))
   (set (reg:CCDSP CCDSP_CC_REGNUM)
	(unspec:CCDSP [(match_dup 0) (match_dup 1)] UNSPEC_WRDSP))
   (set (reg:CCDSP CCDSP_EF_REGNUM)
	(unspec:CCDSP [(match_dup 0) (match_dup 1)] UNSPEC_WRDSP))]
  "ISA_HAS_DSP"
  "wrdsp\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; RDDSP
(define_insn "mips_rddsp"
  [(set (match_operand:SI 0 "register_operand" "=d")
	(unspec:SI [(match_operand:SI 1 "const_uimm6_operand" "YA")
		    (reg:CCDSP CCDSP_PO_REGNUM)
		    (reg:CCDSP CCDSP_SC_REGNUM)
		    (reg:CCDSP CCDSP_CA_REGNUM)
		    (reg:CCDSP CCDSP_OU_REGNUM)
		    (reg:CCDSP CCDSP_CC_REGNUM)
		    (reg:CCDSP CCDSP_EF_REGNUM)]
		   UNSPEC_RDDSP))]
  "ISA_HAS_DSP"
  "rddsp\t%0,%1"
  [(set_attr "type"	"dspalu")
   (set_attr "mode"	"SI")])

;; Table 2-7. MIPS DSP ASE Instructions: Indexed-Load
;; L*X
(define_expand "mips_lbux"
  [(match_operand:SI 0 "register_operand")
   (match_operand 1 "pmode_register_operand")
   (match_operand:SI 2 "register_operand")]
  "ISA_HAS_DSP"
{
  operands[2] = convert_to_mode (Pmode, operands[2], false);
  emit_insn (PMODE_INSN (gen_mips_lbux_extsi,
			 (operands[0], operands[1], operands[2])));
  DONE;
})

(define_insn "mips_l<SHORT:size><u>x_ext<GPR:mode>_<P:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=d")
   	(any_extend:GPR
	  (mem:SHORT (plus:P (match_operand:P 1 "register_operand" "d")
			     (match_operand:P 2 "register_operand" "d")))))]
  "ISA_HAS_L<SHORT:SIZE><U>X"
  "l<SHORT:size><u>x\t%0,%2(%1)"
  [(set_attr "type"	"load")
   (set_attr "mode"	"<GPR:MODE>")])

(define_expand "mips_lhx"
  [(match_operand:SI 0 "register_operand")
   (match_operand 1 "pmode_register_operand")
   (match_operand:SI 2 "register_operand")]
  "ISA_HAS_DSP"
{
  operands[2] = convert_to_mode (Pmode, operands[2], false);
  emit_insn (PMODE_INSN (gen_mips_lhx_extsi,
			 (operands[0], operands[1], operands[2])));
  DONE;
})

(define_expand "mips_l<size>x"
  [(match_operand:GPR 0 "register_operand")
   (match_operand 1 "pmode_register_operand")
   (match_operand:SI 2 "register_operand")]
  "ISA_HAS_DSP"
{
  operands[2] = convert_to_mode (Pmode, operands[2], false);
  emit_insn (PMODE_INSN (gen_mips_l<size>x,
			 (operands[0], operands[1], operands[2])));
  DONE;
})

(define_insn "mips_l<GPR:size>x_<P:mode>"
  [(set (match_operand:GPR 0 "register_operand" "=d")
	(mem:GPR (plus:P (match_operand:P 1 "register_operand" "d")
			 (match_operand:P 2 "register_operand" "d"))))]
  "ISA_HAS_L<GPR:SIZE>X"
  "l<GPR:size>x\t%0,%2(%1)"
  [(set_attr "type"	"load")
   (set_attr "mode"	"<GPR:MODE>")])

(define_insn "*mips_lw<u>x_<P:mode>_ext"
  [(set (match_operand:DI 0 "register_operand" "=d")
   	(any_extend:DI
	  (mem:SI (plus:P (match_operand:P 1 "register_operand" "d")
			     (match_operand:P 2 "register_operand" "d")))))]
  "ISA_HAS_LW<U>X && TARGET_64BIT"
  "lw<u>x\t%0,%2(%1)"
  [(set_attr "type"	"load")
   (set_attr "mode"	"DI")])

;; Table 2-8. MIPS DSP ASE Instructions: Branch
;; BPOSGE32
(define_insn "mips_bposge"
  [(set (pc)
	(if_then_else (ge (reg:CCDSP CCDSP_PO_REGNUM)
			  (match_operand:SI 1 "immediate_operand" "I"))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  "ISA_HAS_DSP"
  "%*bposge%1\t%0%/"
  [(set_attr "type"	"branch")])

(define_expand "mips_madd<u>"
  [(set (match_operand:DI 0 "register_operand")
	(plus:DI
	 (mult:DI (any_extend:DI (match_operand:SI 2 "register_operand"))
		  (any_extend:DI (match_operand:SI 3 "register_operand")))
	 (match_operand:DI 1 "register_operand")))]
  "ISA_HAS_DSP && !TARGET_64BIT")

(define_expand "mips_msub<u>"
  [(set (match_operand:DI 0 "register_operand")
	(minus:DI
	 (match_operand:DI 1 "register_operand")
	 (mult:DI (any_extend:DI (match_operand:SI 2 "register_operand"))
		  (any_extend:DI (match_operand:SI 3 "register_operand")))))]
  "ISA_HAS_DSP && !TARGET_64BIT")
