;; Iterators for RISC-V 'V' Extension for GNU compiler.
;; Copyright (C) 2022-2023 Free Software Foundation, Inc.
;; Contributed by Juzhe Zhong (juzhe.zhong@rivai.ai), RiVAI Technologies Ltd.

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

(define_c_enum "unspec" [
  UNSPEC_VSETVL
  UNSPEC_VUNDEF
  UNSPEC_VPREDICATE
  UNSPEC_VLMAX
  UNSPEC_STRIDED

  ;; It's used to specify ordered/unorderd operation.
  UNSPEC_ORDERED
  UNSPEC_UNORDERED

  ;; vmulh/vmulhu/vmulhsu
  UNSPEC_VMULHS
  UNSPEC_VMULHU
  UNSPEC_VMULHSU

  UNSPEC_VADC
  UNSPEC_VSBC
  UNSPEC_VMADC
  UNSPEC_VMSBC
  UNSPEC_OVERFLOW

  UNSPEC_VNCLIP
  UNSPEC_VNCLIPU
  UNSPEC_VSSRL
  UNSPEC_VSSRA
  UNSPEC_VAADDU
  UNSPEC_VAADD
  UNSPEC_VASUBU
  UNSPEC_VASUB
  UNSPEC_VSMUL

  UNSPEC_VMSBF
  UNSPEC_VMSIF
  UNSPEC_VMSOF
  UNSPEC_VIOTA
])

(define_mode_iterator V [
  VNx1QI VNx2QI VNx4QI VNx8QI VNx16QI VNx32QI (VNx64QI "TARGET_MIN_VLEN > 32")
  VNx1HI VNx2HI VNx4HI VNx8HI VNx16HI (VNx32HI "TARGET_MIN_VLEN > 32")
  VNx1SI VNx2SI VNx4SI VNx8SI (VNx16SI "TARGET_MIN_VLEN > 32")
  (VNx1DI "TARGET_MIN_VLEN > 32") (VNx2DI "TARGET_MIN_VLEN > 32")
  (VNx4DI "TARGET_MIN_VLEN > 32") (VNx8DI "TARGET_MIN_VLEN > 32")
  (VNx1SF "TARGET_VECTOR_ELEN_FP_32")
  (VNx2SF "TARGET_VECTOR_ELEN_FP_32")
  (VNx4SF "TARGET_VECTOR_ELEN_FP_32")
  (VNx8SF "TARGET_VECTOR_ELEN_FP_32")
  (VNx16SF "TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN > 32")
  (VNx1DF "TARGET_VECTOR_ELEN_FP_64")
  (VNx2DF "TARGET_VECTOR_ELEN_FP_64")
  (VNx4DF "TARGET_VECTOR_ELEN_FP_64")
  (VNx8DF "TARGET_VECTOR_ELEN_FP_64")
])

(define_mode_iterator VI [
  VNx1QI VNx2QI VNx4QI VNx8QI VNx16QI VNx32QI (VNx64QI "TARGET_MIN_VLEN > 32")
  VNx1HI VNx2HI VNx4HI VNx8HI VNx16HI (VNx32HI "TARGET_MIN_VLEN > 32")
  VNx1SI VNx2SI VNx4SI VNx8SI (VNx16SI "TARGET_MIN_VLEN > 32")
  (VNx1DI "TARGET_MIN_VLEN > 32") (VNx2DI "TARGET_MIN_VLEN > 32")
  (VNx4DI "TARGET_MIN_VLEN > 32") (VNx8DI "TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VFULLI [
  VNx1QI VNx2QI VNx4QI VNx8QI VNx16QI VNx32QI (VNx64QI "TARGET_MIN_VLEN > 32")
  VNx1HI VNx2HI VNx4HI VNx8HI VNx16HI (VNx32HI "TARGET_MIN_VLEN > 32")
  VNx1SI VNx2SI VNx4SI VNx8SI (VNx16SI "TARGET_MIN_VLEN > 32")
  (VNx1DI "TARGET_FULL_V") (VNx2DI "TARGET_FULL_V")
  (VNx4DI "TARGET_FULL_V") (VNx8DI "TARGET_FULL_V")
])

(define_mode_iterator VI_QHS [
  VNx1QI VNx2QI VNx4QI VNx8QI VNx16QI VNx32QI (VNx64QI "TARGET_MIN_VLEN > 32")
  VNx1HI VNx2HI VNx4HI VNx8HI VNx16HI (VNx32HI "TARGET_MIN_VLEN > 32")
  VNx1SI VNx2SI VNx4SI VNx8SI (VNx16SI "TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VI_D [
  (VNx1DI "TARGET_MIN_VLEN > 32") (VNx2DI "TARGET_MIN_VLEN > 32")
  (VNx4DI "TARGET_MIN_VLEN > 32") (VNx8DI "TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VFULLI_D [
  (VNx1DI "TARGET_FULL_V") (VNx2DI "TARGET_FULL_V")
  (VNx4DI "TARGET_FULL_V") (VNx8DI "TARGET_FULL_V")
])

(define_mode_iterator VNX1_QHSD [
  VNx1QI VNx1HI VNx1SI
  (VNx1DI "TARGET_MIN_VLEN > 32")
  (VNx1SF "TARGET_VECTOR_ELEN_FP_32")
  (VNx1DF "TARGET_VECTOR_ELEN_FP_64")
])

(define_mode_iterator VNX2_QHSD [
  VNx2QI VNx2HI VNx2SI
  (VNx2DI "TARGET_MIN_VLEN > 32")
  (VNx2SF "TARGET_VECTOR_ELEN_FP_32")
  (VNx2DF "TARGET_VECTOR_ELEN_FP_64")
])

(define_mode_iterator VNX4_QHSD [
  VNx4QI VNx4HI VNx4SI
  (VNx4DI "TARGET_MIN_VLEN > 32")
  (VNx4SF "TARGET_VECTOR_ELEN_FP_32")
  (VNx4DF "TARGET_VECTOR_ELEN_FP_64")
])

(define_mode_iterator VNX8_QHSD [
  VNx8QI VNx8HI VNx8SI
  (VNx8DI "TARGET_MIN_VLEN > 32")
  (VNx8SF "TARGET_VECTOR_ELEN_FP_32")
  (VNx8DF "TARGET_VECTOR_ELEN_FP_64")
])

(define_mode_iterator VNX16_QHS [
  VNx16QI VNx16HI (VNx16SI "TARGET_MIN_VLEN > 32")
  (VNx16SF "TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VNX32_QH [
  VNx32QI (VNx32HI "TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VNX64_Q [
  (VNx64QI "TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VNX1_QHSDI [
  VNx1QI VNx1HI VNx1SI
  (VNx1DI "TARGET_64BIT && TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VNX2_QHSDI [
  VNx2QI VNx2HI VNx2SI
  (VNx2DI "TARGET_64BIT && TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VNX4_QHSDI [
  VNx4QI VNx4HI VNx4SI
  (VNx4DI "TARGET_64BIT && TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VNX8_QHSDI [
  VNx8QI VNx8HI VNx8SI
  (VNx8DI "TARGET_64BIT && TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VNX16_QHSI [
  VNx16QI VNx16HI (VNx16SI "TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VNX32_QHI [
  VNx32QI (VNx32HI "TARGET_MIN_VLEN > 32")
])

(define_mode_iterator V_WHOLE [
  (VNx4QI "TARGET_MIN_VLEN == 32") VNx8QI VNx16QI VNx32QI (VNx64QI "TARGET_MIN_VLEN > 32")
  (VNx2HI "TARGET_MIN_VLEN == 32") VNx4HI VNx8HI VNx16HI (VNx32HI "TARGET_MIN_VLEN > 32")
  (VNx1SI "TARGET_MIN_VLEN == 32") VNx2SI VNx4SI VNx8SI (VNx16SI "TARGET_MIN_VLEN > 32")
  (VNx1DI "TARGET_MIN_VLEN > 32") (VNx2DI "TARGET_MIN_VLEN > 32")
  (VNx4DI "TARGET_MIN_VLEN > 32") (VNx8DI "TARGET_MIN_VLEN > 32")
  (VNx1SF "TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN == 32")
  (VNx2SF "TARGET_VECTOR_ELEN_FP_32")
  (VNx4SF "TARGET_VECTOR_ELEN_FP_32")
  (VNx8SF "TARGET_VECTOR_ELEN_FP_32")
  (VNx16SF "TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN > 32")
  (VNx1DF "TARGET_VECTOR_ELEN_FP_64")
  (VNx2DF "TARGET_VECTOR_ELEN_FP_64")
  (VNx4DF "TARGET_VECTOR_ELEN_FP_64")
  (VNx8DF "TARGET_VECTOR_ELEN_FP_64")
])

(define_mode_iterator V_FRACT [
  VNx1QI VNx2QI (VNx4QI "TARGET_MIN_VLEN > 32")
  VNx1HI (VNx2HI "TARGET_MIN_VLEN > 32")
  (VNx1SI "TARGET_MIN_VLEN > 32")
  (VNx1SF "TARGET_VECTOR_ELEN_FP_32 && TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VB [
  VNx1BI VNx2BI VNx4BI VNx8BI VNx16BI VNx32BI
  (VNx64BI "TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VWEXTI [
  VNx1HI VNx2HI VNx4HI VNx8HI VNx16HI (VNx32HI "TARGET_MIN_VLEN > 32")
  VNx1SI VNx2SI VNx4SI VNx8SI (VNx16SI "TARGET_MIN_VLEN > 32")
  (VNx1DI "TARGET_MIN_VLEN > 32") (VNx2DI "TARGET_MIN_VLEN > 32")
  (VNx4DI "TARGET_MIN_VLEN > 32") (VNx8DI "TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VQEXTI [
  VNx1SI VNx2SI VNx4SI VNx8SI (VNx16SI "TARGET_MIN_VLEN > 32")
  (VNx1DI "TARGET_MIN_VLEN > 32") (VNx2DI "TARGET_MIN_VLEN > 32")
  (VNx4DI "TARGET_MIN_VLEN > 32") (VNx8DI "TARGET_MIN_VLEN > 32")
])

(define_mode_iterator VOEXTI [
  (VNx1DI "TARGET_MIN_VLEN > 32") (VNx2DI "TARGET_MIN_VLEN > 32")
  (VNx4DI "TARGET_MIN_VLEN > 32") (VNx8DI "TARGET_MIN_VLEN > 32")
])

(define_mode_attr VM [
  (VNx1QI "VNx1BI") (VNx2QI "VNx2BI") (VNx4QI "VNx4BI") (VNx8QI "VNx8BI") (VNx16QI "VNx16BI") (VNx32QI "VNx32BI") (VNx64QI "VNx64BI")
  (VNx1HI "VNx1BI") (VNx2HI "VNx2BI") (VNx4HI "VNx4BI") (VNx8HI "VNx8BI") (VNx16HI "VNx16BI") (VNx32HI "VNx32BI")
  (VNx1SI "VNx1BI") (VNx2SI "VNx2BI") (VNx4SI "VNx4BI") (VNx8SI "VNx8BI") (VNx16SI "VNx16BI")
  (VNx1DI "VNx1BI") (VNx2DI "VNx2BI") (VNx4DI "VNx4BI") (VNx8DI "VNx8BI")
  (VNx1SF "VNx1BI") (VNx2SF "VNx2BI") (VNx4SF "VNx4BI") (VNx8SF "VNx8BI") (VNx16SF "VNx16BI")
  (VNx1DF "VNx1BI") (VNx2DF "VNx2BI") (VNx4DF "VNx4BI") (VNx8DF "VNx8BI")
])

(define_mode_attr vm [
  (VNx1QI "vnx1bi") (VNx2QI "vnx2bi") (VNx4QI "vnx4bi") (VNx8QI "vnx8bi") (VNx16QI "vnx16bi") (VNx32QI "vnx32bi") (VNx64QI "vnx64bi")
  (VNx1HI "vnx1bi") (VNx2HI "vnx2bi") (VNx4HI "vnx4bi") (VNx8HI "vnx8bi") (VNx16HI "vnx16bi") (VNx32HI "vnx32bi")
  (VNx1SI "vnx1bi") (VNx2SI "vnx2bi") (VNx4SI "vnx4bi") (VNx8SI "vnx8bi") (VNx16SI "vnx16bi")
  (VNx1DI "vnx1bi") (VNx2DI "vnx2bi") (VNx4DI "vnx4bi") (VNx8DI "vnx8bi")
  (VNx1SF "vnx1bi") (VNx2SF "vnx2bi") (VNx4SF "vnx4bi") (VNx8SF "vnx8bi") (VNx16SF "vnx16bi")
  (VNx1DF "vnx1bi") (VNx2DF "vnx2bi") (VNx4DF "vnx4bi") (VNx8DF "vnx8bi")
])

(define_mode_attr VEL [
  (VNx1QI "QI") (VNx2QI "QI") (VNx4QI "QI") (VNx8QI "QI") (VNx16QI "QI") (VNx32QI "QI") (VNx64QI "QI")
  (VNx1HI "HI") (VNx2HI "HI") (VNx4HI "HI") (VNx8HI "HI") (VNx16HI "HI") (VNx32HI "HI")
  (VNx1SI "SI") (VNx2SI "SI") (VNx4SI "SI") (VNx8SI "SI") (VNx16SI "SI")
  (VNx1DI "DI") (VNx2DI "DI") (VNx4DI "DI") (VNx8DI "DI")
  (VNx1SF "SF") (VNx2SF "SF") (VNx4SF "SF") (VNx8SF "SF") (VNx16SF "SF")
  (VNx1DF "DF") (VNx2DF "DF") (VNx4DF "DF") (VNx8DF "DF")
])

(define_mode_attr VSUBEL [
  (VNx1HI "QI") (VNx2HI "QI") (VNx4HI "QI") (VNx8HI "QI") (VNx16HI "QI") (VNx32HI "QI")
  (VNx1SI "HI") (VNx2SI "HI") (VNx4SI "HI") (VNx8SI "HI") (VNx16SI "HI")
  (VNx1DI "SI") (VNx2DI "SI") (VNx4DI "SI") (VNx8DI "SI")
  (VNx1SF "HF") (VNx2SF "HF") (VNx4SF "HF") (VNx8SF "HF") (VNx16SF "HF")
  (VNx1DF "SF") (VNx2DF "SF") (VNx4DF "SF") (VNx8DF "SF")
])

(define_mode_attr sew [
  (VNx1QI "8") (VNx2QI "8") (VNx4QI "8") (VNx8QI "8") (VNx16QI "8") (VNx32QI "8") (VNx64QI "8")
  (VNx1HI "16") (VNx2HI "16") (VNx4HI "16") (VNx8HI "16") (VNx16HI "16") (VNx32HI "16")
  (VNx1SI "32") (VNx2SI "32") (VNx4SI "32") (VNx8SI "32") (VNx16SI "32")
  (VNx1DI "64") (VNx2DI "64") (VNx4DI "64") (VNx8DI "64")
  (VNx1SF "32") (VNx2SF "32") (VNx4SF "32") (VNx8SF "32") (VNx16SF "32")
  (VNx1DF "64") (VNx2DF "64") (VNx4DF "64") (VNx8DF "64")
])

(define_mode_attr V_DOUBLE_TRUNC [
  (VNx1HI "VNx1QI") (VNx2HI "VNx2QI")  (VNx4HI "VNx4QI")  (VNx8HI "VNx8QI")  
  (VNx16HI "VNx16QI") (VNx32HI "VNx32QI")
  (VNx1SI "VNx1HI") (VNx2SI "VNx2HI") (VNx4SI "VNx4HI") (VNx8SI "VNx8HI") 
  (VNx16SI "VNx16HI")
  (VNx1DI "VNx1SI") (VNx2DI "VNx2SI") (VNx4DI "VNx4SI") (VNx8DI "VNx8SI")
])

(define_mode_attr V_QUAD_TRUNC [
  (VNx1SI "VNx1QI") (VNx2SI "VNx2QI") (VNx4SI "VNx4QI") (VNx8SI "VNx8QI") 
  (VNx16SI "VNx16QI")
  (VNx1DI "VNx1HI") (VNx2DI "VNx2HI")
  (VNx4DI "VNx4HI") (VNx8DI "VNx8HI")
])

(define_mode_attr V_OCT_TRUNC [
  (VNx1DI "VNx1QI") (VNx2DI "VNx2QI") (VNx4DI "VNx4QI") (VNx8DI "VNx8QI")
])

(define_int_iterator ORDER [UNSPEC_ORDERED UNSPEC_UNORDERED])

(define_int_iterator VMULH [UNSPEC_VMULHS UNSPEC_VMULHU UNSPEC_VMULHSU])

(define_int_iterator VNCLIP [UNSPEC_VNCLIP UNSPEC_VNCLIPU])

(define_int_iterator VSAT_OP [UNSPEC_VAADDU UNSPEC_VAADD
			      UNSPEC_VASUBU UNSPEC_VASUB UNSPEC_VSMUL
			      UNSPEC_VSSRL UNSPEC_VSSRA])

(define_int_iterator VSAT_ARITH_OP [UNSPEC_VAADDU UNSPEC_VAADD
			      	    UNSPEC_VASUBU UNSPEC_VASUB UNSPEC_VSMUL])
(define_int_iterator VSAT_SHIFT_OP [UNSPEC_VSSRL UNSPEC_VSSRA])

(define_int_iterator VMISC [UNSPEC_VMSBF UNSPEC_VMSIF UNSPEC_VMSOF])

(define_int_attr order [
  (UNSPEC_ORDERED "o") (UNSPEC_UNORDERED "u")
])

(define_int_attr v_su [(UNSPEC_VMULHS "") (UNSPEC_VMULHU "u") (UNSPEC_VMULHSU "su")
		       (UNSPEC_VNCLIP "") (UNSPEC_VNCLIPU "u")])
(define_int_attr sat_op [(UNSPEC_VAADDU "aaddu") (UNSPEC_VAADD "aadd")
			 (UNSPEC_VASUBU "asubu") (UNSPEC_VASUB "asub")
			 (UNSPEC_VSMUL "smul") (UNSPEC_VSSRL "ssrl")
			 (UNSPEC_VSSRA "ssra")])
(define_int_attr sat_insn_type [(UNSPEC_VAADDU "vaalu") (UNSPEC_VAADD "vaalu")
			 	(UNSPEC_VASUBU "vaalu") (UNSPEC_VASUB "vaalu")
			 	(UNSPEC_VSMUL "vsmul") (UNSPEC_VSSRL "vsshift")
			 	(UNSPEC_VSSRA "vsshift") (UNSPEC_VNCLIP "vnclip")
				(UNSPEC_VNCLIPU "vnclip")])

(define_int_attr misc_op [(UNSPEC_VMSBF "sbf") (UNSPEC_VMSIF "sif") (UNSPEC_VMSOF "sof")])

(define_code_iterator any_int_binop [plus minus and ior xor ashift ashiftrt lshiftrt
  smax umax smin umin mult div udiv mod umod
])

(define_code_iterator any_int_unop [neg not])

(define_code_iterator any_commutative_binop [plus and ior xor
  smax umax smin umin mult
])

(define_code_iterator any_non_commutative_binop [minus div udiv mod umod])

(define_code_iterator any_sat_int_binop [ss_plus ss_minus us_plus us_minus])
(define_code_iterator sat_int_plus_binop [ss_plus us_plus])
(define_code_iterator sat_int_minus_binop [ss_minus us_minus])

(define_code_iterator any_widen_binop [plus minus mult])
(define_code_iterator plus_minus [plus minus])

(define_code_attr macc_nmsac [(plus "macc") (minus "nmsac")])
(define_code_attr madd_nmsub [(plus "madd") (minus "nmsub")])

(define_code_iterator and_ior [and ior])
(define_code_attr ninsn [(and "nand") (ior "nor") (xor "xnor")])

(define_code_attr binop_rhs1_predicate [
			(plus "register_operand")
			(minus "vector_arith_operand")
			(ior "register_operand")
			(xor "register_operand")
			(and "register_operand")
			(ashift "register_operand")
			(ashiftrt "register_operand")
			(lshiftrt "register_operand")
			(smin "register_operand")
			(smax "register_operand")
			(umin "register_operand")
			(umax "register_operand")
			(mult "register_operand")
			(div "register_operand")
			(mod "register_operand")
			(udiv "register_operand")
			(umod "register_operand")
			(ss_plus "register_operand")
			(us_plus "register_operand")
			(ss_minus "register_operand")
			(us_minus "register_operand")])

(define_code_attr binop_rhs2_predicate [
			(plus "vector_arith_operand")
			(minus "vector_neg_arith_operand")
			(ior "vector_arith_operand")
			(xor "vector_arith_operand")
			(and "vector_arith_operand")
			(ashift "vector_shift_operand")
			(ashiftrt "vector_shift_operand")
			(lshiftrt "vector_shift_operand")
			(smin "register_operand")
			(smax "register_operand")
			(umin "register_operand")
			(umax "register_operand")
			(mult "register_operand")
			(div "register_operand")
			(mod "register_operand")
			(udiv "register_operand")
			(umod "register_operand")
			(ss_plus "vector_arith_operand")
			(us_plus "vector_arith_operand")
			(ss_minus "vector_neg_arith_operand")
			(us_minus "register_operand")])

(define_code_attr binop_rhs1_constraint [
			(plus "vr,vr,vr,vr,vr,vr")
			(minus "vr,vr,vr,vr,vi,vi")
			(ior "vr,vr,vr,vr,vr,vr")
			(xor "vr,vr,vr,vr,vr,vr")
			(and "vr,vr,vr,vr,vr,vr")
			(ashift "vr,vr,vr,vr,vr,vr")
			(ashiftrt "vr,vr,vr,vr,vr,vr")
			(lshiftrt "vr,vr,vr,vr,vr,vr")
			(smin "vr,vr,vr,vr,vr,vr")
			(smax "vr,vr,vr,vr,vr,vr")
			(umin "vr,vr,vr,vr,vr,vr")
			(umax "vr,vr,vr,vr,vr,vr")
			(mult "vr,vr,vr,vr,vr,vr")
			(div "vr,vr,vr,vr,vr,vr")
			(mod "vr,vr,vr,vr,vr,vr")
			(udiv "vr,vr,vr,vr,vr,vr")
			(umod "vr,vr,vr,vr,vr,vr")])

(define_code_attr binop_rhs2_constraint [
			(plus "vr,vr,vi,vi,vr,vr")
			(minus "vr,vr,vj,vj,vr,vr")
			(ior "vr,vr,vi,vi,vr,vr")
			(xor "vr,vr,vi,vi,vr,vr")
			(and "vr,vr,vi,vi,vr,vr")
			(ashift "vr,vr,vk,vk,vr,vr")
			(ashiftrt "vr,vr,vk,vk,vr,vr")
			(lshiftrt "vr,vr,vk,vk,vr,vr")
			(smin "vr,vr,vr,vr,vr,vr")
			(smax "vr,vr,vr,vr,vr,vr")
			(umin "vr,vr,vr,vr,vr,vr")
			(umax "vr,vr,vr,vr,vr,vr")
			(mult "vr,vr,vr,vr,vr,vr")
			(div "vr,vr,vr,vr,vr,vr")
			(mod "vr,vr,vr,vr,vr,vr")
			(udiv "vr,vr,vr,vr,vr,vr")
			(umod "vr,vr,vr,vr,vr,vr")
			(ss_plus "vr,vr,vi,vi")
			(us_plus "vr,vr,vi,vi")
			(ss_minus "vr,vr,vj,vj")
			(us_minus "vr,vr,vr,vr")])

(define_code_attr int_binop_insn_type [
			(plus "vialu")
			(minus "vialu")
			(ior "vialu")
			(xor "vialu")
			(and "vialu")
			(ashift "vshift")
			(ashiftrt "vshift")
			(lshiftrt "vshift")
			(smin "viminmax")
			(smax "viminmax")
			(umin "viminmax")
			(umax "viminmax")
			(mult "vimul")
			(div "vidiv")
			(mod "vidiv")
			(udiv "vidiv")
			(umod "vidiv")
			(ss_plus "vsalu")
			(us_plus "vsalu")
			(ss_minus "vsalu")
			(us_minus "vsalu")])

(define_code_attr widen_binop_insn_type [
			(plus "walu")
			(minus "walu")
			(mult "wmul")])

;; <binop_vi_variant_insn> expands to the insn name of binop matching constraint rhs1 is immediate.
;; minus is negated as vadd and ss_minus is negated as vsadd, others remain <insn>.
(define_code_attr binop_vi_variant_insn [(ashift "sll.vi")
			       (ashiftrt "sra.vi")
			       (lshiftrt "srl.vi")
			       (div "div.vv")
			       (mod "rem.vv")
			       (udiv "divu.vv")
			       (umod "remu.vv")
			       (ior "or.vi")
			       (xor "xor.vi")
			       (and "and.vi")
			       (plus "add.vi")
			       (minus "add.vi")
			       (smin "min.vv")
			       (smax "max.vv")
			       (umin "minu.vv")
			       (umax "maxu.vv")
			       (mult "mul.vv")
			       (ss_plus "sadd.vi")
			       (us_plus "saddu.vi")
			       (ss_minus "sadd.vi")
			       (us_minus "ssubu.vv")])

;; <binop_reverse_vi_variant_insn> expands to the insn name of binop matching constraint rhs2 is immediate.
;; minus is reversed as vrsub, others remain <insn>.
(define_code_attr binop_reverse_vi_variant_insn [(ashift "sll.vv")
			       (ashiftrt "sra.vv")
			       (lshiftrt "srl.vv")
			       (div "div.vv")
			       (mod "rem.vv")
			       (udiv "divu.vv")
			       (umod "remu.vv")
			       (ior "or.vv")
			       (xor "xor.vv")
			       (and "and.vv")
			       (plus "add.vv")
			       (minus "rsub.vi")
			       (smin "min.vv")
			       (smax "max.vv")
			       (umin "minu.vv")
			       (umax "maxu.vv")
			       (mult "mul.vv")])

(define_code_attr binop_vi_variant_op [(ashift "%3,%v4")
			     (ashiftrt "%3,%v4")
			     (lshiftrt "%3,%v4")
			     (div "%3,%4")
			     (mod "%3,%4")
			     (udiv "%3,%4")
			     (umod "%3,%4")
			     (ior "%3,%v4")
			     (xor "%3,%v4")
			     (and "%3,%v4")
			     (plus "%3,%v4")
			     (minus "%3,%V4")
			     (smin "%3,%4")
			     (smax "%3,%4")
			     (umin "%3,%4")
			     (umax "%3,%4")
			     (mult "%3,%4")
			     (ss_plus "%3,%v4")
			     (us_plus "%3,%v4")
			     (ss_minus "%3,%V4")
			     (us_minus "%3,%4")])

(define_code_attr binop_reverse_vi_variant_op [(ashift "%3,%4")
			      (ashiftrt "%3,%4")
			      (lshiftrt "%3,%4")
			      (div "%3,%4")
			      (mod "%3,%4")
			      (udiv "%3,%4")
			      (umod "%3,%4")
			      (ior "%3,%4")
			      (xor "%3,%4")
			      (and "%3,%4")
			      (plus "%3,%4")
			      (minus "%4,%v3")
			      (smin "%3,%4")
			      (smax "%3,%4")
			      (umin "%3,%4")
			      (umax "%3,%4")
			      (mult "%3,%4")])

(define_code_attr sz [(sign_extend "s") (zero_extend "z")])
