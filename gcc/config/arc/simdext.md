;; Machine description of the Synopsys DesignWare ARC cpu for GNU C compiler
;; Copyright (C) 2007-2023 Free Software Foundation, Inc.

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
  ;; Va, Vb, Vc builtins
  UNSPEC_ARC_SIMD_VADDAW
  UNSPEC_ARC_SIMD_VADDW
  UNSPEC_ARC_SIMD_VAVB
  UNSPEC_ARC_SIMD_VAVRB
  UNSPEC_ARC_SIMD_VDIFAW
  UNSPEC_ARC_SIMD_VDIFW
  UNSPEC_ARC_SIMD_VMAXAW
  UNSPEC_ARC_SIMD_VMAXW
  UNSPEC_ARC_SIMD_VMINAW
  UNSPEC_ARC_SIMD_VMINW
  UNSPEC_ARC_SIMD_VMULAW
  UNSPEC_ARC_SIMD_VMULFAW
  UNSPEC_ARC_SIMD_VMULFW
  UNSPEC_ARC_SIMD_VMULW
  UNSPEC_ARC_SIMD_VSUBAW
  UNSPEC_ARC_SIMD_VSUBW
  UNSPEC_ARC_SIMD_VSUMMW
  UNSPEC_ARC_SIMD_VAND
  UNSPEC_ARC_SIMD_VANDAW
  UNSPEC_ARC_SIMD_VBIC
  UNSPEC_ARC_SIMD_VBICAW
  UNSPEC_ARC_SIMD_VOR
  UNSPEC_ARC_SIMD_VXOR
  UNSPEC_ARC_SIMD_VXORAW
  UNSPEC_ARC_SIMD_VEQW
  UNSPEC_ARC_SIMD_VLEW
  UNSPEC_ARC_SIMD_VLTW
  UNSPEC_ARC_SIMD_VNEW
  UNSPEC_ARC_SIMD_VMR1AW
  UNSPEC_ARC_SIMD_VMR1W
  UNSPEC_ARC_SIMD_VMR2AW
  UNSPEC_ARC_SIMD_VMR2W
  UNSPEC_ARC_SIMD_VMR3AW
  UNSPEC_ARC_SIMD_VMR3W
  UNSPEC_ARC_SIMD_VMR4AW
  UNSPEC_ARC_SIMD_VMR4W
  UNSPEC_ARC_SIMD_VMR5AW
  UNSPEC_ARC_SIMD_VMR5W
  UNSPEC_ARC_SIMD_VMR6AW
  UNSPEC_ARC_SIMD_VMR6W
  UNSPEC_ARC_SIMD_VMR7AW
  UNSPEC_ARC_SIMD_VMR7W
  UNSPEC_ARC_SIMD_VMRB
  UNSPEC_ARC_SIMD_VH264F
  UNSPEC_ARC_SIMD_VH264FT
  UNSPEC_ARC_SIMD_VH264FW
  UNSPEC_ARC_SIMD_VVC1F
  UNSPEC_ARC_SIMD_VVC1FT
  ;; Va, Vb, rc/limm builtins
  UNSPEC_ARC_SIMD_VBADDW
  UNSPEC_ARC_SIMD_VBMAXW
  UNSPEC_ARC_SIMD_VBMINW
  UNSPEC_ARC_SIMD_VBMULAW
  UNSPEC_ARC_SIMD_VBMULFW
  UNSPEC_ARC_SIMD_VBMULW
  UNSPEC_ARC_SIMD_VBRSUBW
  UNSPEC_ARC_SIMD_VBSUBW

  ;; Va, Vb, Ic builtins
  UNSPEC_ARC_SIMD_VASRW
  UNSPEC_ARC_SIMD_VSR8
  UNSPEC_ARC_SIMD_VSR8AW

  ;; Va, Vb, Ic builtins
  UNSPEC_ARC_SIMD_VASRRWi
  UNSPEC_ARC_SIMD_VASRSRWi
  UNSPEC_ARC_SIMD_VASRWi
  UNSPEC_ARC_SIMD_VASRPWBi
  UNSPEC_ARC_SIMD_VASRRPWBi
  UNSPEC_ARC_SIMD_VSR8AWi
  UNSPEC_ARC_SIMD_VSR8i

  ;; Va, Vb, u8 (simm) builtins
  UNSPEC_ARC_SIMD_VMVAW
  UNSPEC_ARC_SIMD_VMVW
  UNSPEC_ARC_SIMD_VMVZW
  UNSPEC_ARC_SIMD_VD6TAPF

  ;; Va, rlimm, u8 (simm) builtins
  UNSPEC_ARC_SIMD_VMOVAW
  UNSPEC_ARC_SIMD_VMOVW
  UNSPEC_ARC_SIMD_VMOVZW

  ;; Va, Vb builtins
  UNSPEC_ARC_SIMD_VABSAW
  UNSPEC_ARC_SIMD_VABSW
  UNSPEC_ARC_SIMD_VADDSUW
  UNSPEC_ARC_SIMD_VSIGNW
  UNSPEC_ARC_SIMD_VEXCH1
  UNSPEC_ARC_SIMD_VEXCH2
  UNSPEC_ARC_SIMD_VEXCH4
  UNSPEC_ARC_SIMD_VUPBAW
  UNSPEC_ARC_SIMD_VUPBW
  UNSPEC_ARC_SIMD_VUPSBAW
  UNSPEC_ARC_SIMD_VUPSBW

  UNSPEC_ARC_SIMD_VDIRUN
  UNSPEC_ARC_SIMD_VDORUN
  UNSPEC_ARC_SIMD_VDIWR
  UNSPEC_ARC_SIMD_VDOWR

  UNSPEC_ARC_SIMD_VREC
  UNSPEC_ARC_SIMD_VRUN
  UNSPEC_ARC_SIMD_VRECRUN
  UNSPEC_ARC_SIMD_VENDREC

  UNSPEC_ARC_SIMD_VCAST
  UNSPEC_ARC_SIMD_VINTI
  ])

;; Scheduler descriptions for the simd instructions
(define_insn_reservation "simd_lat_0_insn" 1
  (eq_attr "type" "simd_dma, simd_vstore, simd_vcontrol")
  "issue+simd_unit")

(define_insn_reservation "simd_lat_1_insn" 2
       (eq_attr "type" "simd_vcompare, simd_vlogic,
			simd_vmove_else_zero, simd_varith_1cycle")
  "issue+simd_unit, nothing")

(define_insn_reservation "simd_lat_2_insn" 3
       (eq_attr "type" "simd_valign, simd_vpermute,
			simd_vpack, simd_varith_2cycle")
  "issue+simd_unit, nothing*2")

(define_insn_reservation "simd_lat_3_insn" 4
       (eq_attr "type" "simd_valign_with_acc, simd_vpack_with_acc,
			simd_vlogic_with_acc, simd_vload128,
			simd_vmove_with_acc, simd_vspecial_3cycle,
			simd_varith_with_acc")
  "issue+simd_unit, nothing*3")

(define_insn_reservation "simd_lat_4_insn" 5
       (eq_attr "type" "simd_vload, simd_vmove, simd_vspecial_4cycle")
  "issue+simd_unit, nothing*4")

(define_expand "movv8hi"
  [(set (match_operand:V8HI 0 "general_operand" "")
	(match_operand:V8HI 1 "general_operand" ""))]
  ""
  "
{
  /* Everything except mem = const or mem = mem can be done easily.  */

  if (GET_CODE (operands[0]) == MEM && GET_CODE(operands[1]) == MEM)
    operands[1] = force_reg (V8HImode, operands[1]);
}")

;; This pattern should appear before the movv8hi_insn pattern
(define_insn "vld128_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand" "=v")
	(mem:V8HI (plus:SI (zero_extend:SI (vec_select:HI (match_operand:V8HI 1 "vector_register_operand"  "v")
							  (parallel [(match_operand:SI 2 "immediate_operand" "L")])))
			   (match_operand:SI 3 "immediate_operand" "P"))))]
 "TARGET_SIMD_SET"
 "vld128\\t%0,[i%2,%3]"
 [(set_attr "type" "simd_vload128")
  (set_attr "length" "4")
  (set_attr "cond" "nocond")]
)

(define_insn "vst128_insn"
  [(set	(mem:V8HI (plus:SI (zero_extend:SI (vec_select:HI (match_operand:V8HI 0 "vector_register_operand"  "v")
							  (parallel [(match_operand:SI 1 "immediate_operand" "L")])))
			   (match_operand:SI 2 "immediate_operand" "P")))
	(match_operand:V8HI 3 "vector_register_operand" "=v"))]
 "TARGET_SIMD_SET"
 "vst128\\t%3,[i%1,%2]"
 [(set_attr "type" "simd_vstore")
  (set_attr "length" "4")
  (set_attr "cond" "nocond")]
)

(define_insn "vst64_insn"
  [(set	(mem:V4HI
	 (plus:SI
	  (zero_extend:SI
	   (vec_select:HI (match_operand:V8HI 0 "vector_register_operand"  "v")
			  (parallel
			   [(match_operand:SI 1 "immediate_operand" "L")])))
	  (match_operand:SI 2 "immediate_operand" "P")))
	(vec_select:V4HI
	 (match_operand:V8HI 3 "vector_register_operand" "=v")
	 (parallel [(const_int 0) (const_int 1) (const_int 2) (const_int 3)])))]
 "TARGET_SIMD_SET"
 "vst64\\t%3,[i%1,%2]"
 [(set_attr "type" "simd_vstore")
  (set_attr "length" "4")
  (set_attr "cond" "nocond")]
)

(define_insn "movv8hi_insn"
  [(set (match_operand:V8HI 0 "vector_register_or_memory_operand" "=v,m,v")
	(match_operand:V8HI 1 "vector_register_or_memory_operand" "m,v,v"))]
  "TARGET_SIMD_SET && !(GET_CODE (operands[0]) == MEM && GET_CODE(operands[1]) == MEM)"
  "@
    vld128r\\t%0,%1
    vst128r\\t%1,%0
    vmvzw\\t%0,%1,0xffff"
  [(set_attr "type" "simd_vload128,simd_vstore,simd_vmove_else_zero")
   (set_attr "length" "8,8,4")
   (set_attr "cond" "nocond, nocond, nocond")])

(define_insn "movti_insn"
  [(set (match_operand:TI 0 "vector_register_or_memory_operand" "=v,m,v")
	(match_operand:TI 1 "vector_register_or_memory_operand" "m,v,v"))]
  ""
  "@
    vld128r\\t%0,%1
    vst128r\\t%1,%0
    vmvzw\\t%0,%1,0xffff"
  [(set_attr "type" "simd_vload128,simd_vstore,simd_vmove_else_zero")
   (set_attr "length" "8,8,4")
   (set_attr "cond" "nocond, nocond, nocond")])

;; V V V Insns
(define_insn "vaddaw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VADDAW))]
  "TARGET_SIMD_SET"
  "vaddaw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vaddw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VADDW))]
  "TARGET_SIMD_SET"
  "vaddw\\t%0,%1,2"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vavb_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VAVB))]
  "TARGET_SIMD_SET"
  "vavb\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vavrb_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VAVRB))]
  "TARGET_SIMD_SET"
  "vavrb\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vdifaw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VDIFAW))]
  "TARGET_SIMD_SET"
  "vdifaw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vdifw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VDIFW))]
  "TARGET_SIMD_SET"
  "vdifw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmaxaw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMAXAW))]
  "TARGET_SIMD_SET"
  "vmaxaw\\t%0,%1,2"
  [(set_attr "type" "simd_varith_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmaxw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMAXW))]
  "TARGET_SIMD_SET"
  "vmaxw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vminaw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMINAW))]
  "TARGET_SIMD_SET"
  "vminaw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vminw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMINW))]
  "TARGET_SIMD_SET"
  "vminw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmulaw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMULAW))]
  "TARGET_SIMD_SET"
  "vmulaw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmulfaw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMULFAW))]
  "TARGET_SIMD_SET"
  "vmulfaw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmulfw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMULFW))]
  "TARGET_SIMD_SET"
  "vmulfw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_2cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmulw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMULW))]
  "TARGET_SIMD_SET"
  "vmulw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_2cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vsubaw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VSUBAW))]
  "TARGET_SIMD_SET"
  "vsubaw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vsubw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VSUBW))]
  "TARGET_SIMD_SET"
  "vsubw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vsummw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VSUMMW))]
  "TARGET_SIMD_SET"
  "vsummw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_2cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vand_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VAND))]
  "TARGET_SIMD_SET"
  "vand\\t%0,%1,%2"
  [(set_attr "type" "simd_vlogic")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vandaw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VANDAW))]
  "TARGET_SIMD_SET"
  "vandaw\\t%0,%1,%2"
  [(set_attr "type" "simd_vlogic_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vbic_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VBIC))]
  "TARGET_SIMD_SET"
  "vbic\\t%0,%1,%2"
  [(set_attr "type" "simd_vlogic")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vbicaw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VBICAW))]
  "TARGET_SIMD_SET"
  "vbicaw\\t%0,%1,%2"
  [(set_attr "type" "simd_vlogic_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vor_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VOR))]
  "TARGET_SIMD_SET"
  "vor\\t%0,%1,%2"
  [(set_attr "type" "simd_vlogic")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vxor_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VXOR))]
  "TARGET_SIMD_SET"
  "vxor\\t%0,%1,%2"
  [(set_attr "type" "simd_vlogic")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vxoraw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VXORAW))]
  "TARGET_SIMD_SET"
  "vxoraw\\t%0,%1,%2"
  [(set_attr "type" "simd_vlogic_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "veqw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VEQW))]
  "TARGET_SIMD_SET"
  "veqw\\t%0,%1,%2"
  [(set_attr "type" "simd_vcompare")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vlew_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VLEW))]
  "TARGET_SIMD_SET"
  "vlew\\t%0,%1,%2"
  [(set_attr "type" "simd_vcompare")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vltw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VLTW))]
  "TARGET_SIMD_SET"
  "vltw\\t%0,%1,%2"
  [(set_attr "type" "simd_vcompare")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vnew_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VNEW))]
  "TARGET_SIMD_SET"
  "vnew\\t%0,%1,%2"
  [(set_attr "type" "simd_vcompare")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmr1aw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMR1AW))]
  "TARGET_SIMD_SET"
  "vmr1aw\\t%0,%1,%2"
  [(set_attr "type" "simd_valign_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmr1w_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMR1W))]
  "TARGET_SIMD_SET"
  "vmr1w\\t%0,%1,%2"
  [(set_attr "type" "simd_valign")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmr2aw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMR2AW))]
  "TARGET_SIMD_SET"
  "vmr2aw\\t%0,%1,%2"
  [(set_attr "type" "simd_valign_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmr2w_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMR2W))]
  "TARGET_SIMD_SET"
  "vmr2w\\t%0,%1,%2"
  [(set_attr "type" "simd_valign")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmr3aw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMR3AW))]
  "TARGET_SIMD_SET"
  "vmr3aw\\t%0,%1,%2"
  [(set_attr "type" "simd_valign_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmr3w_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMR3W))]
  "TARGET_SIMD_SET"
  "vmr3w\\t%0,%1,%2"
  [(set_attr "type" "simd_valign")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmr4aw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMR4AW))]
  "TARGET_SIMD_SET"
  "vmr4aw\\t%0,%1,%2"
  [(set_attr "type" "simd_valign_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmr4w_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMR4W))]
  "TARGET_SIMD_SET"
  "vmr4w\\t%0,%1,%2"
  [(set_attr "type" "simd_valign")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmr5aw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMR5AW))]
  "TARGET_SIMD_SET"
  "vmr5aw\\t%0,%1,%2"
  [(set_attr "type" "simd_valign_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmr5w_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMR5W))]
  "TARGET_SIMD_SET"
  "vmr5w\\t%0,%1,%2"
  [(set_attr "type" "simd_valign")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmr6aw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMR6AW))]
  "TARGET_SIMD_SET"
  "vmr6aw\\t%0,%1,%2"
  [(set_attr "type" "simd_valign_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmr6w_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMR6W))]
  "TARGET_SIMD_SET"
  "vmr6w\\t%0,%1,%2"
  [(set_attr "type" "simd_valign")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmr7aw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMR7AW))]
  "TARGET_SIMD_SET"
  "vmr7aw\\t%0,%1,%2"
  [(set_attr "type" "simd_valign_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmr7w_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMR7W))]
  "TARGET_SIMD_SET"
  "vmr7w\\t%0,%1,%2"
  [(set_attr "type" "simd_valign")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmrb_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VMRB))]
  "TARGET_SIMD_SET"
  "vmrb\\t%0,%1,%2"
  [(set_attr "type" "simd_valign")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vh264f_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VH264F))]
  "TARGET_SIMD_SET"
  "vh264f\\t%0,%1,%2"
  [(set_attr "type" "simd_vspecial_3cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vh264ft_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VH264FT))]
  "TARGET_SIMD_SET"
  "vh264ft\\t%0,%1,%2"
  [(set_attr "type" "simd_vspecial_3cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vh264fw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VH264FW))]
  "TARGET_SIMD_SET"
  "vh264fw\\t%0,%1,%2"
  [(set_attr "type" "simd_vspecial_3cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vvc1f_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VVC1F))]
  "TARGET_SIMD_SET"
  "vvc1f\\t%0,%1,%2"
  [(set_attr "type" "simd_vspecial_3cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vvc1ft_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:V8HI 2 "vector_register_operand" "v")]
		     UNSPEC_ARC_SIMD_VVC1FT))]
  "TARGET_SIMD_SET"
  "vvc1ft\\t%0,%1,%2"
  [(set_attr "type" "simd_vspecial_3cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])



;;---
;; V V r/limm Insns
(define_insn "vbaddw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "nonmemory_operand" "r")]
		     UNSPEC_ARC_SIMD_VBADDW))]
  "TARGET_SIMD_SET"
  "vbaddw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vbmaxw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "nonmemory_operand" "r")]
		     UNSPEC_ARC_SIMD_VBMAXW))]
  "TARGET_SIMD_SET"
  "vbmaxw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vbminw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "nonmemory_operand" "r")]
		     UNSPEC_ARC_SIMD_VBMINW))]
  "TARGET_SIMD_SET"
  "vbminw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vbmulaw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "nonmemory_operand" "r")]
		     UNSPEC_ARC_SIMD_VBMULAW))]
  "TARGET_SIMD_SET"
  "vbmulaw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vbmulfw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "nonmemory_operand" "r")]
		     UNSPEC_ARC_SIMD_VBMULFW))]
  "TARGET_SIMD_SET"
  "vbmulfw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_2cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vbmulw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "nonmemory_operand" "r")]
		     UNSPEC_ARC_SIMD_VBMULW))]
  "TARGET_SIMD_SET"
  "vbmulw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_2cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vbrsubw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "nonmemory_operand" "r")]
		     UNSPEC_ARC_SIMD_VBRSUBW))]
  "TARGET_SIMD_SET"
  "vbrsubw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vbsubw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "nonmemory_operand" "r")]
		     UNSPEC_ARC_SIMD_VBSUBW))]
  "TARGET_SIMD_SET"
  "vbsubw\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])
; Va, Vb, Ic instructions

; Va, Vb, u6 instructions
(define_insn "vasrrwi_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "immediate_operand" "L")]
		     UNSPEC_ARC_SIMD_VASRRWi))]
  "TARGET_SIMD_SET"
  "vasrrwi\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_2cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vasrsrwi_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "immediate_operand" "L")]
		     UNSPEC_ARC_SIMD_VASRSRWi))]
  "TARGET_SIMD_SET"
  "vasrsrwi\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_2cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vasrwi_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "immediate_operand" "L")]
		     UNSPEC_ARC_SIMD_VASRWi))]
  "TARGET_SIMD_SET"
  "vasrwi\\t%0,%1,%2"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vasrpwbi_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "immediate_operand" "L")]
		     UNSPEC_ARC_SIMD_VASRPWBi))]
  "TARGET_SIMD_SET"
  "vasrpwbi\\t%0,%1,%2"
  [(set_attr "type" "simd_vpack")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vasrrpwbi_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "immediate_operand" "L")]
		     UNSPEC_ARC_SIMD_VASRRPWBi))]
  "TARGET_SIMD_SET"
  "vasrrpwbi\\t%0,%1,%2"
  [(set_attr "type" "simd_vpack")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vsr8awi_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "immediate_operand" "L")]
		     UNSPEC_ARC_SIMD_VSR8AWi))]
  "TARGET_SIMD_SET"
  "vsr8awi\\t%0,%1,%2"
  [(set_attr "type" "simd_valign_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vsr8i_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "immediate_operand" "L")]
		     UNSPEC_ARC_SIMD_VSR8i))]
  "TARGET_SIMD_SET"
  "vsr8i\\t%0,%1,%2"
  [(set_attr "type" "simd_valign")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

;; Va, Vb, u8 (simm) insns

(define_insn "vmvaw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "immediate_operand" "P")]
		     UNSPEC_ARC_SIMD_VMVAW))]
  "TARGET_SIMD_SET"
  "vmvaw\\t%0,%1,%2"
  [(set_attr "type" "simd_vmove_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmvw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "immediate_operand" "P")]
		     UNSPEC_ARC_SIMD_VMVW))]
  "TARGET_SIMD_SET"
  "vmvw\\t%0,%1,%2"
  [(set_attr "type" "simd_vmove")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmvzw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "immediate_operand" "P")]
		     UNSPEC_ARC_SIMD_VMVZW))]
  "TARGET_SIMD_SET"
  "vmvzw\\t%0,%1,%2"
  [(set_attr "type" "simd_vmove_else_zero")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vd6tapf_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "immediate_operand" "P")]
		     UNSPEC_ARC_SIMD_VD6TAPF))]
  "TARGET_SIMD_SET"
  "vd6tapf\\t%0,%1,%2"
  [(set_attr "type" "simd_vspecial_4cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

;; Va, rlimm, u8 (simm) insns
(define_insn "vmovaw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:SI 1 "nonmemory_operand"  "r")
		      (match_operand:SI 2 "immediate_operand" "P")]
		     UNSPEC_ARC_SIMD_VMOVAW))]
  "TARGET_SIMD_SET"
  "vmovaw\\t%0,%1,%2"
  [(set_attr "type" "simd_vmove_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmovw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:SI 1 "nonmemory_operand"  "r")
		      (match_operand:SI 2 "immediate_operand" "P")]
		     UNSPEC_ARC_SIMD_VMOVW))]
  "TARGET_SIMD_SET"
  "vmovw\\t%0,%1,%2"
  [(set_attr "type" "simd_vmove")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vmovzw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:SI 1 "nonmemory_operand"  "r")
		      (match_operand:SI 2 "immediate_operand" "P")]
		     UNSPEC_ARC_SIMD_VMOVZW))]
  "TARGET_SIMD_SET"
  "vmovzw\\t%0,%1,%2"
  [(set_attr "type" "simd_vmove_else_zero")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

;; Va, rlimm, Ic insns
(define_insn "vsr8_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "immediate_operand" "K")
		      (match_operand:V8HI 3 "vector_register_operand"  "v")]
		     UNSPEC_ARC_SIMD_VSR8))]
  "TARGET_SIMD_SET"
  "vsr8\\t%0,%1,i%2"
  [(set_attr "type" "simd_valign")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vasrw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "immediate_operand" "K")
		      (match_operand:V8HI 3 "vector_register_operand"  "v")]
		     UNSPEC_ARC_SIMD_VASRW))]
  "TARGET_SIMD_SET"
  "vasrw\\t%0,%1,i%2"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vsr8aw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"           "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")
		      (match_operand:SI 2 "immediate_operand" "K")
		      (match_operand:V8HI 3 "vector_register_operand"  "v")]
		     UNSPEC_ARC_SIMD_VSR8AW))]
  "TARGET_SIMD_SET"
  "vsr8aw\\t%0,%1,i%2"
  [(set_attr "type" "simd_valign_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

;; Va, Vb insns
(define_insn "vabsaw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"  "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")]
		     UNSPEC_ARC_SIMD_VABSAW))]
  "TARGET_SIMD_SET"
  "vabsaw\\t%0,%1"
  [(set_attr "type" "simd_varith_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vabsw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"  "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")]
		     UNSPEC_ARC_SIMD_VABSW))]
  "TARGET_SIMD_SET"
  "vabsw\\t%0,%1"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vaddsuw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"  "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")]
		     UNSPEC_ARC_SIMD_VADDSUW))]
  "TARGET_SIMD_SET"
  "vaddsuw\\t%0,%1"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vsignw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"  "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")]
		     UNSPEC_ARC_SIMD_VSIGNW))]
  "TARGET_SIMD_SET"
  "vsignw\\t%0,%1"
  [(set_attr "type" "simd_varith_1cycle")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vexch1_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"  "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")]
		     UNSPEC_ARC_SIMD_VEXCH1))]
  "TARGET_SIMD_SET"
  "vexch1\\t%0,%1"
  [(set_attr "type" "simd_vpermute")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vexch2_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"  "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")]
		     UNSPEC_ARC_SIMD_VEXCH2))]
  "TARGET_SIMD_SET"
  "vexch2\\t%0,%1"
  [(set_attr "type" "simd_vpermute")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vexch4_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"  "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")]
		     UNSPEC_ARC_SIMD_VEXCH4))]
  "TARGET_SIMD_SET"
  "vexch4\\t%0,%1"
  [(set_attr "type" "simd_vpermute")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vupbaw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"  "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")]
		     UNSPEC_ARC_SIMD_VUPBAW))]
  "TARGET_SIMD_SET"
  "vupbaw\\t%0,%1"
  [(set_attr "type" "simd_vpack_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vupbw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"  "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")]
		     UNSPEC_ARC_SIMD_VUPBW))]
  "TARGET_SIMD_SET"
  "vupbw\\t%0,%1"
  [(set_attr "type" "simd_vpack")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vupsbaw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"  "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")]
		     UNSPEC_ARC_SIMD_VUPSBAW))]
  "TARGET_SIMD_SET"
  "vupsbaw\\t%0,%1"
  [(set_attr "type" "simd_vpack_with_acc")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vupsbw_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand"  "=v")
	(unspec:V8HI [(match_operand:V8HI 1 "vector_register_operand"  "v")]
		     UNSPEC_ARC_SIMD_VUPSBW))]
  "TARGET_SIMD_SET"
  "vupsbw\\t%0,%1"
  [(set_attr "type" "simd_vpack")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

; DMA setup instructions
(define_insn "vdirun_insn"
  [(set (match_operand:SI 0 "arc_simd_dma_register_operand"           "=d")
	(unspec_volatile:SI [(match_operand:SI 1 "nonmemory_operand"  "r")
			     (match_operand:SI 2 "nonmemory_operand" "r")]
			    UNSPEC_ARC_SIMD_VDIRUN))]
  "TARGET_SIMD_SET"
  "vdirun\\t%1,%2"
  [(set_attr "type" "simd_dma")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vdorun_insn"
  [(set (match_operand:SI 0 "arc_simd_dma_register_operand"              "=d")
	(unspec_volatile:SI [(match_operand:SI 1 "nonmemory_operand"     "r")
			     (match_operand:SI 2 "nonmemory_operand"     "r")]
			    UNSPEC_ARC_SIMD_VDORUN))]
  "TARGET_SIMD_SET"
  "vdorun\\t%1,%2"
  [(set_attr "type" "simd_dma")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vdiwr_insn"
  [(set (match_operand:SI 0 "arc_simd_dma_register_operand"           "=d,d")
	(unspec_volatile:SI [(match_operand:SI 1 "nonmemory_operand"  "r,Cal")]
			    UNSPEC_ARC_SIMD_VDIWR))]
  "TARGET_SIMD_SET"
  "vdiwr\\t%0,%1"
  [(set_attr "type" "simd_dma")
   (set_attr "length" "4,8")
   (set_attr "cond" "nocond,nocond")])

(define_insn "vdowr_insn"
  [(set (match_operand:SI 0 "arc_simd_dma_register_operand"           "=d,d")
	(unspec_volatile:SI [(match_operand:SI 1 "nonmemory_operand"  "r,Cal")]
			    UNSPEC_ARC_SIMD_VDOWR))]
  "TARGET_SIMD_SET"
  "vdowr\\t%0,%1"
  [(set_attr "type" "simd_dma")
   (set_attr "length" "4,8")
   (set_attr "cond" "nocond,nocond")])

;; vector record and run instructions
(define_insn "vrec_insn"
  [(unspec_volatile [(match_operand:SI 0 "nonmemory_operand"  "r")]
		    UNSPEC_ARC_SIMD_VREC)]
  "TARGET_SIMD_SET"
  "vrec\\t%0"
  [(set_attr "type" "simd_vcontrol")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vrun_insn"
  [(unspec_volatile [(match_operand:SI 0 "nonmemory_operand"  "r")]
		    UNSPEC_ARC_SIMD_VRUN)]
  "TARGET_SIMD_SET"
  "vrun\\t%0"
  [(set_attr "type" "simd_vcontrol")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vrecrun_insn"
  [(unspec_volatile [(match_operand:SI 0 "nonmemory_operand"  "r")]
		    UNSPEC_ARC_SIMD_VRECRUN)]
  "TARGET_SIMD_SET"
  "vrecrun\\t%0"
  [(set_attr "type" "simd_vcontrol")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vendrec_insn"
  [(unspec_volatile [(match_operand:SI 0 "nonmemory_operand"  "r")]
		    UNSPEC_ARC_SIMD_VENDREC)]
  "TARGET_SIMD_SET"
  "vendrec\\t%0"
  [(set_attr "type" "simd_vcontrol")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vld32wh_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand" "=v")
	(vec_concat:V8HI
	 (zero_extend:V4HI
	  (mem:V4QI
	   (plus:SI
	    (match_operand:SI 1 "immediate_operand" "P")
	    (zero_extend:SI
	     (vec_select:HI
	      (match_operand:V8HI 2 "vector_register_operand"  "v")
	      (parallel [(match_operand:SI 3 "immediate_operand" "L")]))))))
	 (vec_select:V4HI
	  (match_dup 0)
	  (parallel [(const_int 0) (const_int 1) (const_int 2) (const_int 3)])
	  )))]
  "TARGET_SIMD_SET"
  "vld32wh\\t%0,[i%3,%1]"
  [(set_attr "type" "simd_vload")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vld32wl_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand" "=v")
	(vec_concat:V8HI
	 (vec_select:V4HI
	  (match_dup 0)
	  (parallel [(const_int 4) (const_int 5) (const_int 6) (const_int 7)]))
	 (zero_extend:V4HI
	  (mem:V4QI
	   (plus:SI
	    (match_operand:SI 1 "immediate_operand" "P")
	    (zero_extend:SI
	     (vec_select:HI (match_operand:V8HI 2 "vector_register_operand" "v")
			    (parallel
			     [(match_operand:SI 3 "immediate_operand" "L")]))
	     ))))))]
  "TARGET_SIMD_SET"
  "vld32wl\\t%0,[i%3,%1]"
  [(set_attr "type" "simd_vload")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vld64w_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand" "=v")
	(zero_extend:V8HI
	 (mem:V4HI
	  (plus:SI
	   (zero_extend:SI
	    (vec_select:HI (match_operand:V8HI 1 "vector_register_operand"  "v")
			   (parallel [(match_operand:SI 2 "immediate_operand" "L")])))
	   (match_operand:SI 3 "immediate_operand" "P")))))]
 "TARGET_SIMD_SET"
 "vld64w\\t%0,[i%2,%3]"
 [(set_attr "type" "simd_vload")
  (set_attr "length" "4")
  (set_attr "cond" "nocond")]
)

(define_insn "vld64_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand" "=v")
	(vec_concat:V8HI
	 (vec_select:V4HI
	  (match_dup 0)
	  (parallel [(const_int 4) (const_int 5) (const_int 6) (const_int 7)]))
	 (mem:V4HI
	  (plus:SI
	   (match_operand:SI 1 "immediate_operand" "P")
	   (zero_extend:SI
	    (vec_select:HI
	     (match_operand:V8HI 2 "vector_register_operand"  "v")
	     (parallel [(match_operand:SI 3 "immediate_operand" "L")]))
	    )))))]
  "TARGET_SIMD_SET"
  "vld64\\t%0,[i%3,%1]"
  [(set_attr "type" "simd_vload")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vld32_insn"
  [(set (match_operand:V8HI 0 "vector_register_operand" "=v")
	(vec_concat:V8HI
	 (vec_select:V4HI
	  (match_dup 0)
	  (parallel [(const_int 4) (const_int 5) (const_int 6) (const_int 7)]))
	 (vec_concat:V4HI
	  (vec_select:V2HI
	   (match_dup 0)
	   (parallel [(const_int 2) (const_int 3)]))
	  (mem:V2HI
	   (plus:SI
	    (match_operand:SI 1 "immediate_operand" "P")
	    (zero_extend:SI
	     (vec_select:HI
	      (match_operand:V8HI 2 "vector_register_operand"  "v")
	      (parallel [(match_operand:SI 3 "immediate_operand" "L")]))))))))]
  "TARGET_SIMD_SET"
  "vld32\\t%0,[i%3,%1]"
  [(set_attr "type" "simd_vload")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

(define_insn "vst16_n_insn"
  [(set (mem:HI
	 (plus:SI
	  (match_operand:SI 0 "immediate_operand" "P")
	  (zero_extend:SI
	   (vec_select:HI (match_operand:V8HI 1 "vector_register_operand"  "v")
			  (parallel [(match_operand:SI 2 "immediate_operand" "L")])))))
	 (vec_select:HI (match_operand:V8HI 3 "vector_register_operand" "v")
			(parallel [(match_operand:SI 4 "immediate_operand" "L")])))]
 "TARGET_SIMD_SET"
 "vst16_%4\\t%3,[i%2,%0]"
 [(set_attr "type" "simd_vstore")
  (set_attr "length" "4")
  (set_attr "cond" "nocond")])

(define_insn "vst32_n_insn"
  [(set (mem:SI
	 (plus:SI
	  (match_operand:SI 0 "immediate_operand" "P")
	  (zero_extend:SI
	   (vec_select:HI (match_operand:V8HI 1 "vector_register_operand"  "v")
			  (parallel [(match_operand:SI 2 "immediate_operand" "L")])))))
	(vec_select:SI (unspec:V4SI [(match_operand:V8HI 3 "vector_register_operand" "v")]
				    UNSPEC_ARC_SIMD_VCAST)
		       (parallel [(match_operand:SI 4 "immediate_operand" "L")])))]
 "TARGET_SIMD_SET"
 "vst32_%4\\t%3,[i%2,%0]"
 [(set_attr "type" "simd_vstore")
  (set_attr "length" "4")
  (set_attr "cond" "nocond")])

;; SIMD unit interrupt
(define_insn "vinti_insn"
  [(unspec_volatile [(match_operand:SI 0 "nonmemory_operand"  "L")]
		    UNSPEC_ARC_SIMD_VINTI)]
  "TARGET_SIMD_SET"
  "vinti\\t%0"
  [(set_attr "type" "simd_vcontrol")
   (set_attr "length" "4")
   (set_attr "cond" "nocond")])

;; New ARCv2 SIMD extensions

;;64-bit vectors of halwords and words
(define_mode_iterator VWH [V4HI V2SI])

;;double element vectors
(define_mode_iterator VDV [V2HI V2SI])
(define_mode_attr V_addsub [(V2HI "HI") (V2SI "SI")])
(define_mode_attr V_addsub_suffix [(V2HI "2h") (V2SI "")])

;;all vectors
(define_mode_iterator VCT [(V2HI "TARGET_PLUS_DMPY")
			   (V4HI "TARGET_PLUS_QMACW")
			   (V2SI "TARGET_PLUS_QMACW")])
(define_mode_attr V_suffix [(V2HI "2h") (V4HI "4h") (V2SI "2")])

(define_code_iterator EMUVEC [(mult "TARGET_MPYW")
			      (div "TARGET_DIVREM")
			      smax smin])

(define_code_attr voptab [(mult "mul")
			  (div "div")
			  (smin "smin")
			  (smax "smax")])

;; Widening operations.
(define_code_iterator SE [sign_extend zero_extend])
(define_code_attr V_US [(sign_extend "s") (zero_extend "u")])
(define_code_attr V_US_suffix [(sign_extend "") (zero_extend "u")])


;; Move patterns
(define_expand "movv2hi"
  [(set (match_operand:V2HI 0 "move_dest_operand" "")
	(match_operand:V2HI 1 "general_operand" ""))]
  ""
  "{
    if (prepare_move_operands (operands, V2HImode))
         DONE;
   }")

(define_insn_and_split "*movv2hi_insn"
  [(set (match_operand:V2HI 0 "move_dest_operand" "=r,r,r,m")
	(match_operand:V2HI 1 "general_operand"    "i,r,m,r"))]
  "(register_operand (operands[0], V2HImode)
    || register_operand (operands[1], V2HImode))"
  "@
   #
   mov%?\\t%0,%1
   ld%U1%V1\\t%0,%1
   st%U0%V0\\t%1,%0"
  "reload_completed && GET_CODE (operands[1]) == CONST_VECTOR"
  [(set (match_dup 0) (match_dup 2))]
  {
   HOST_WIDE_INT intval = INTVAL (XVECEXP (operands[1], 0, 1)) << 16;
   intval |= INTVAL (XVECEXP (operands[1], 0, 0)) & 0xFFFF;

   operands[0] = gen_rtx_REG (SImode, REGNO (operands[0]));
   operands[2] = GEN_INT (trunc_int_for_mode (intval, SImode));
  }
  [(set_attr "type" "move,move,load,store")
   (set_attr "predicable" "yes,yes,no,no")
   (set_attr "iscompact"  "false,false,false,false")
   ])

(define_expand "movmisalignv2hi"
  [(set (match_operand:V2HI 0 "general_operand" "")
	(match_operand:V2HI 1 "general_operand" ""))]
 "unaligned_access"
 "{
   if (prepare_move_operands (operands, V2HImode))
     DONE;
  }")

(define_expand "mov<mode>"
  [(set (match_operand:VWH 0 "move_dest_operand" "")
	(match_operand:VWH 1 "general_operand" ""))]
  ""
  "{
    if (prepare_move_operands (operands, <MODE>mode))
     DONE;
   }")

(define_insn_and_split "*mov<mode>_insn"
  [(set (match_operand:VWH 0 "move_dest_operand" "=r,r,r,m")
	(match_operand:VWH 1 "general_operand"    "i,r,m,r"))]
  "(register_operand (operands[0], <MODE>mode)
       || register_operand (operands[1], <MODE>mode))"
  "@
   #
   vadd2\\t%0,%1,0
   ldd%U1%V1\\t%0,%1
   std%U0%V0\\t%1,%0"
  "&& reload_completed && arc_split_move_p (operands)"
  [(const_int 0)]
  {
   arc_split_move (operands);
   DONE;
  }
  [(set_attr "type" "move,move,load,store")
   (set_attr "length" "16,8,16,16")])

(define_expand "movmisalign<mode>"
  [(set (match_operand:VWH 0 "general_operand" "")
	(match_operand:VWH 1 "general_operand" ""))]
 "unaligned_access"
 "{
   if (prepare_move_operands (operands, <MODE>mode))
     DONE;
  }")

(define_insn "bswapv2hi2"
  [(set (match_operand:V2HI 0 "register_operand" "=r,r")
	(bswap:V2HI (match_operand:V2HI 1 "nonmemory_operand" "r,i")))]
  "TARGET_V2 && TARGET_SWAP"
  "swape\\t%0,%1"
  [(set_attr "length" "4,8")
   (set_attr "type" "two_cycle_core")])

;; Simple arithmetic insns
(define_insn "add<mode>3"
  [(set (match_operand:VCT 0 "register_operand"          "=r,r")
	(plus:VCT (match_operand:VCT 1 "register_operand" "0,r")
		  (match_operand:VCT 2 "register_operand" "r,r")))]
  "TARGET_PLUS_DMPY"
  "vadd<V_suffix>%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "sub<mode>3"
  [(set (match_operand:VCT 0 "register_operand"           "=r,r")
	(minus:VCT (match_operand:VCT 1 "register_operand" "0,r")
		   (match_operand:VCT 2 "register_operand" "r,r")))]
  "TARGET_PLUS_DMPY"
  "vsub<V_suffix>%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

;; Combined arithmetic ops
(define_insn "addsub<mode>3"
  [(set (match_operand:VDV 0 "register_operand" "=r,r")
	(vec_concat:VDV
	 (plus:<V_addsub>
	  (vec_select:<V_addsub> (match_operand:VDV 1 "register_operand" "0,r")
				 (parallel [(const_int 0)]))
	  (vec_select:<V_addsub> (match_operand:VDV 2 "register_operand" "r,r")
				 (parallel [(const_int 0)])))
	 (minus:<V_addsub>
	  (vec_select:<V_addsub> (match_dup 1) (parallel [(const_int 1)]))
	  (vec_select:<V_addsub> (match_dup 2) (parallel [(const_int 1)])))))]
  "TARGET_PLUS_DMPY"
  "vaddsub<V_addsub_suffix>%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "subadd<mode>3"
  [(set (match_operand:VDV 0 "register_operand" "=r,r")
	(vec_concat:VDV
	 (minus:<V_addsub>
	  (vec_select:<V_addsub> (match_operand:VDV 1 "register_operand" "0,r")
				 (parallel [(const_int 0)]))
	  (vec_select:<V_addsub> (match_operand:VDV 2 "register_operand" "r,r")
				 (parallel [(const_int 0)])))
	 (plus:<V_addsub>
	  (vec_select:<V_addsub> (match_dup 1) (parallel [(const_int 1)]))
	  (vec_select:<V_addsub> (match_dup 2) (parallel [(const_int 1)])))))]
  "TARGET_PLUS_DMPY"
  "vsubadd<V_addsub_suffix>%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "addsubv4hi3"
  [(set (match_operand:V4HI 0 "even_register_operand" "=r,r")
	(vec_concat:V4HI
	 (vec_concat:V2HI
	  (plus:HI
	   (vec_select:HI (match_operand:V4HI 1 "even_register_operand" "0,r")
			  (parallel [(const_int 0)]))
	   (vec_select:HI (match_operand:V4HI 2 "even_register_operand" "r,r")
			  (parallel [(const_int 0)])))
	  (minus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
		    (vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
	 (vec_concat:V2HI
	  (plus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
		   (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
	  (minus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
		    (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))
	 ))]
  "TARGET_PLUS_QMACW"
  "vaddsub4h%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "subaddv4hi3"
  [(set (match_operand:V4HI 0 "even_register_operand" "=r,r")
	(vec_concat:V4HI
	 (vec_concat:V2HI
	  (minus:HI
	   (vec_select:HI (match_operand:V4HI 1 "even_register_operand" "0,r")
			  (parallel [(const_int 0)]))
	   (vec_select:HI (match_operand:V4HI 2 "even_register_operand" "r,r")
			  (parallel [(const_int 0)])))
	  (plus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 1)]))
		   (vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))
	 (vec_concat:V2HI
	  (minus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 2)]))
		    (vec_select:HI (match_dup 2) (parallel [(const_int 2)])))
	  (plus:HI (vec_select:HI (match_dup 1) (parallel [(const_int 3)]))
		   (vec_select:HI (match_dup 2) (parallel [(const_int 3)]))))
	 ))]
  "TARGET_PLUS_QMACW"
  "vsubadd4h%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

;; Multiplication
(define_insn "dmpyh<V_US_suffix>"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(plus:SI
	 (mult:SI
	  (SE:SI
	   (vec_select:HI (match_operand:V2HI 1 "register_operand" "0,r")
			  (parallel [(const_int 0)])))
	  (SE:SI
	   (vec_select:HI (match_operand:V2HI 2 "register_operand" "r,r")
			  (parallel [(const_int 0)]))))
	 (mult:SI
	  (SE:SI (vec_select:HI (match_dup 1) (parallel [(const_int 1)])))
	  (SE:SI (vec_select:HI (match_dup 2) (parallel [(const_int 1)]))))))
   (set (reg:DI ARCV2_ACC)
	(zero_extend:DI
	 (plus:SI
	  (mult:SI
	   (SE:SI (vec_select:HI (match_dup 1) (parallel [(const_int 0)])))
	   (SE:SI (vec_select:HI (match_dup 2) (parallel [(const_int 0)]))))
	  (mult:SI
	   (SE:SI (vec_select:HI (match_dup 1) (parallel [(const_int 1)])))
	   (SE:SI (vec_select:HI (match_dup 2) (parallel [(const_int 1)])))))))]
  "TARGET_PLUS_DMPY"
  "dmpyh<V_US_suffix>%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

;; We can use dmac as well here.  To be investigated which version
;; brings more.
(define_expand "sdot_prodv2hi"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:V2HI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")
   (match_operand:SI 3 "register_operand" "")]
  "TARGET_PLUS_DMPY"
{
 rtx t = gen_reg_rtx (SImode);
 emit_insn (gen_dmpyh (t, operands[1], operands[2]));
 emit_insn (gen_addsi3 (operands[0], operands[3], t));
 DONE;
})

(define_expand "udot_prodv2hi"
  [(match_operand:SI 0 "register_operand" "")
   (match_operand:V2HI 1 "register_operand" "")
   (match_operand:V2HI 2 "register_operand" "")
   (match_operand:SI 3 "register_operand" "")]
  "TARGET_PLUS_DMPY"
{
 rtx t = gen_reg_rtx (SImode);
 emit_insn (gen_dmpyhu (t, operands[1], operands[2]));
 emit_insn (gen_addsi3 (operands[0], operands[3], t));
 DONE;
})

(define_expand "sdot_prodv4hi"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")
   (match_operand:V2SI 3 "register_operand" "")]
  "TARGET_PLUS_MACD"
{
 rtx acc_reg  = gen_rtx_REG  (V2SImode, ACC_REG_FIRST);
 rtx op1_low  = gen_lowpart  (V2HImode, operands[1]);
 rtx op1_high = gen_highpart (V2HImode, operands[1]);
 rtx op2_low  = gen_lowpart  (V2HImode, operands[2]);
 rtx op2_high = gen_highpart (V2HImode, operands[2]);

 emit_move_insn (acc_reg, operands[3]);
 emit_insn (gen_arc_vec_smac_v2hiv2si_zero (op1_low, op2_low));
 emit_insn (gen_arc_vec_smac_v2hiv2si (operands[0], op1_high, op2_high));
 DONE;
})

(define_expand "udot_prodv4hi"
  [(match_operand:V2SI 0 "register_operand" "")
   (match_operand:V4HI 1 "register_operand" "")
   (match_operand:V4HI 2 "register_operand" "")
   (match_operand:V2SI 3 "register_operand" "")]
  "TARGET_PLUS_MACD"
{
 rtx acc_reg  = gen_rtx_REG  (V2SImode, ACC_REG_FIRST);
 rtx op1_low  = gen_lowpart  (V2HImode, operands[1]);
 rtx op1_high = gen_highpart (V2HImode, operands[1]);
 rtx op2_low  = gen_lowpart  (V2HImode, operands[2]);
 rtx op2_high = gen_highpart (V2HImode, operands[2]);

 emit_move_insn (acc_reg, operands[3]);
 emit_insn (gen_arc_vec_umac_v2hiv2si_zero (op1_low, op2_low));
 emit_insn (gen_arc_vec_umac_v2hiv2si (operands[0], op1_high, op2_high));
 DONE;
})

(define_insn "arc_vec_<V_US>mult_lo_v4hi"
 [(set (match_operand:V2SI 0 "even_register_operand"                     "=r,r")
       (mult:V2SI (SE:V2SI (vec_select:V2HI
			    (match_operand:V4HI 1 "even_register_operand" "0,r")
			    (parallel [(const_int 0) (const_int 1)])))
		  (SE:V2SI (vec_select:V2HI
			    (match_operand:V4HI 2 "even_register_operand" "r,r")
			    (parallel [(const_int 0) (const_int 1)])))))
  (set (reg:V2SI ARCV2_ACC)
       (mult:V2SI (SE:V2SI
		   (vec_select:V2HI (match_dup 1)
				    (parallel [(const_int 0) (const_int 1)])))
		  (SE:V2SI
		   (vec_select:V2HI (match_dup 2)
				    (parallel [(const_int 0) (const_int 1)])))))
  ]
  "TARGET_PLUS_MACD"
  "vmpy2h<V_US_suffix>%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "arc_vec_<V_US>multacc_lo_v4hi"
  [(set (reg:V2SI ARCV2_ACC)
	(mult:V2SI (SE:V2SI (vec_select:V2HI
			     (match_operand:V4HI 0 "even_register_operand" "r")
			     (parallel [(const_int 0) (const_int 1)])))
		   (SE:V2SI (vec_select:V2HI
			     (match_operand:V4HI 1 "even_register_operand" "r")
			     (parallel [(const_int 0) (const_int 1)])))))
  ]
  "TARGET_PLUS_MACD"
  "vmpy2h<V_US_suffix>%?\\t0,%0,%1"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "no")
   (set_attr "cond" "nocond")])

(define_expand "vec_widen_<V_US>mult_lo_v4hi"
 [(set (match_operand:V2SI 0 "even_register_operand"                 "")
       (mult:V2SI (SE:V2SI (vec_select:V2HI
			    (match_operand:V4HI 1 "even_register_operand" "")
			    (parallel [(const_int 0) (const_int 1)])))
		  (SE:V2SI (vec_select:V2HI
			    (match_operand:V4HI 2 "even_register_operand" "")
			    (parallel [(const_int 0) (const_int 1)])))))]
  "TARGET_PLUS_QMACW"
  {
     emit_insn (gen_arc_vec_<V_US>mult_lo_v4hi (operands[0],
						operands[1],
						operands[2]));
     DONE;
  }
)

(define_insn "arc_vec_<V_US>mult_hi_v4hi"
 [(set (match_operand:V2SI 0 "even_register_operand"                     "=r,r")
       (mult:V2SI (SE:V2SI (vec_select:V2HI
			    (match_operand:V4HI 1 "even_register_operand" "0,r")
			    (parallel [(const_int 2) (const_int 3)])))
		  (SE:V2SI (vec_select:V2HI
			    (match_operand:V4HI 2 "even_register_operand" "r,r")
			    (parallel [(const_int 2) (const_int 3)])))))
  (set (reg:V2SI ARCV2_ACC)
       (mult:V2SI (SE:V2SI
		   (vec_select:V2HI (match_dup 1)
				    (parallel [(const_int 2) (const_int 3)])))
		  (SE:V2SI
		   (vec_select:V2HI (match_dup 2)
				    (parallel [(const_int 2) (const_int 3)])))))
  ]
  "TARGET_PLUS_QMACW"
  "vmpy2h<V_US_suffix>%?\\t%0,%R1,%R2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_expand "vec_widen_<V_US>mult_hi_v4hi"
 [(set (match_operand:V2SI 0 "even_register_operand")
       (mult:V2SI (SE:V2SI (vec_select:V2HI
			    (match_operand:V4HI 1 "even_register_operand")
			    (parallel [(const_int 2) (const_int 3)])))
		  (SE:V2SI (vec_select:V2HI
			    (match_operand:V4HI 2 "even_register_operand")
			    (parallel [(const_int 2) (const_int 3)])))))]
  "TARGET_PLUS_QMACW"
  {
     emit_insn (gen_arc_vec_<V_US>mult_hi_v4hi (operands[0],
						operands[1],
						operands[2]));
     DONE;
  }
)

(define_insn "arc_vec_<V_US>mac_v2hiv2si"
 [(set (match_operand:V2SI 0 "even_register_operand"     "=r,Ral,r")
       (plus:V2SI
	(mult:V2SI
	 (SE:V2SI (match_operand:V2HI 1 "register_operand" "0,  r,r"))
	 (SE:V2SI (match_operand:V2HI 2 "register_operand" "r,  r,r")))
	(reg:V2SI ARCV2_ACC)))
  (set (reg:V2SI ARCV2_ACC)
       (plus:V2SI
	(mult:V2SI (SE:V2SI (match_dup 1))
		   (SE:V2SI (match_dup 2)))
	(reg:V2SI ARCV2_ACC)))
  ]
  "TARGET_PLUS_MACD"
  "@
   vmac2h<V_US_suffix>%?\\t%0,%1,%2
   vmac2h<V_US_suffix>%?\\t0,%1,%2
   vmac2h<V_US_suffix>%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no,no")])

(define_insn "arc_vec_<V_US>mac_v2hiv2si_zero"
 [(set (reg:V2SI ARCV2_ACC)
       (plus:V2SI
	(mult:V2SI (SE:V2SI (match_operand:V2HI 0 "register_operand" "r"))
		   (SE:V2SI (match_operand:V2HI 1 "register_operand" "r")))
	(reg:V2SI ARCV2_ACC)))]
  "TARGET_PLUS_MACD"
  "vmac2h<V_US_suffix>%?\\t0,%0,%1"
  [(set_attr "length" "4")
   (set_attr "type" "multi")])

;; Builtins
(define_insn "dmach"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(unspec:SI [(match_operand:V2HI 1 "register_operand" "0,r")
		    (match_operand:V2HI 2 "register_operand" "r,r")
		    (reg:DI ARCV2_ACC)]
		   UNSPEC_ARC_DMACH))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_PLUS_DMPY"
  "dmach%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "dmachu"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(unspec:SI [(match_operand:V2HI 1 "register_operand" "0,r")
		    (match_operand:V2HI 2 "register_operand" "r,r")
		    (reg:DI ARCV2_ACC)]
		   UNSPEC_ARC_DMACHU))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_PLUS_DMPY"
  "dmachu%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "dmacwh"
  [(set (match_operand:DI 0 "even_register_operand" "=r,r")
	(unspec:DI [(match_operand:V2SI 1 "even_register_operand" "0,r")
		    (match_operand:V2HI 2 "register_operand"      "r,r")
		    (reg:DI ARCV2_ACC)]
		   UNSPEC_ARC_DMACWH))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_PLUS_QMACW"
  "dmacwh%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "dmacwhu"
  [(set (match_operand:DI 0 "register_operand" "=r,r")
	(unspec:DI [(match_operand:V2SI 1 "even_register_operand" "0,r")
		    (match_operand:V2HI 2 "register_operand"      "r,r")
		    (reg:DI ARCV2_ACC)]
		   UNSPEC_ARC_DMACWHU))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_PLUS_QMACW"
  "dmacwhu%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "vmac2h"
  [(set (match_operand:V2SI 0 "even_register_operand" "=r,r")
	(unspec:V2SI [(match_operand:V2HI 1 "register_operand" "0,r")
		      (match_operand:V2HI 2 "register_operand" "r,r")
		      (reg:DI ARCV2_ACC)]
		     UNSPEC_ARC_VMAC2H))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_PLUS_MACD"
  "vmac2h%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "vmac2hu"
  [(set (match_operand:V2SI 0 "even_register_operand" "=r,r")
	(unspec:V2SI [(match_operand:V2HI 1 "register_operand" "0,r")
		      (match_operand:V2HI 2 "register_operand" "r,r")
		      (reg:DI ARCV2_ACC)]
		   UNSPEC_ARC_VMAC2HU))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_PLUS_MACD"
  "vmac2hu%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "vmpy2h"
  [(set (match_operand:V2SI 0 "even_register_operand" "=r,r")
	(unspec:V2SI [(match_operand:V2HI 1 "register_operand" "0,r")
		      (match_operand:V2HI 2 "register_operand" "r,r")]
		     UNSPEC_ARC_VMPY2H))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_PLUS_MACD"
  "vmpy2h%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "vmpy2hu"
  [(set (match_operand:V2SI 0 "even_register_operand" "=r,r")
	(unspec:V2SI [(match_operand:V2HI 1 "register_operand" "0,r")
		      (match_operand:V2HI 2 "register_operand" "r,r")]
		     UNSPEC_ARC_VMPY2HU))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_PLUS_MACD"
  "vmpy2hu%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "qmach"
  [(set (match_operand:DI 0 "even_register_operand" "=r,r")
	(unspec:DI [(match_operand:V4HI 1 "even_register_operand" "0,r")
		    (match_operand:V4HI 2 "even_register_operand" "r,r")
		    (reg:DI ARCV2_ACC)]
		     UNSPEC_ARC_QMACH))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_PLUS_QMACW"
  "qmach%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "qmachu"
  [(set (match_operand:DI 0 "even_register_operand" "=r,r")
	(unspec:DI [(match_operand:V4HI 1 "even_register_operand" "0,r")
		    (match_operand:V4HI 2 "even_register_operand" "r,r")
		    (reg:DI ARCV2_ACC)]
		   UNSPEC_ARC_QMACHU))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_PLUS_QMACW"
  "qmachu%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "qmpyh"
  [(set (match_operand:DI 0 "even_register_operand" "=r,r")
	(unspec:DI [(match_operand:V4HI 1 "even_register_operand" "0,r")
		    (match_operand:V4HI 2 "even_register_operand" "r,r")]
		     UNSPEC_ARC_QMPYH))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_PLUS_QMACW"
  "qmpyh%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

(define_insn "qmpyhu"
  [(set (match_operand:DI 0 "even_register_operand" "=r,r")
	(unspec:DI [(match_operand:V4HI 1 "even_register_operand" "0,r")
		    (match_operand:V4HI 2 "even_register_operand" "r,r")]
		   UNSPEC_ARC_QMPYHU))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_PLUS_QMACW"
  "qmpyhu%?\\t%0,%1,%2"
  [(set_attr "length" "4")
   (set_attr "type" "multi")
   (set_attr "predicable" "yes,no")
   (set_attr "cond" "canuse,nocond")])

;; Emulated vector instructions.
(define_insn_and_split "<voptab>v2si3"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(EMUVEC:V2SI (match_operand:V2SI 1 "register_operand" "r")
		     (match_operand:V2SI 2 "nonmemory_operand" "ri")))]
  ""
  "#"
  "reload_completed"
  [(const_int 0)]
  {
   rtx high_dest = gen_highpart (SImode, operands[0]);
   rtx low_dest = gen_lowpart (SImode, operands[0]);
   rtx high_op1 = gen_highpart (SImode, operands[1]);
   rtx low_op1 = gen_lowpart (SImode, operands[1]);
   rtx high_op2 = gen_highpart (SImode, operands[2]);
   rtx low_op2 = gen_lowpart (SImode, operands[2]);
   emit_insn (gen_<voptab>si3 (low_dest, low_op1, low_op2));
   emit_insn (gen_<voptab>si3 (high_dest, high_op1, high_op2));
   DONE;
  }
  [(set_attr "length" "12")
   (set_attr "type" "multi")])

(define_expand "neg<mode>2"
  [(set (match_operand:VCT 0 "register_operand")
	(neg:VCT (match_operand:VCT 1 "register_operand")))]
  "TARGET_PLUS_DMPY"
  "")

(define_insn "*neg<mode>2"
  [(set (match_operand:VCT 0 "register_operand" "=r")
	(neg:VCT (match_operand:VCT 1 "register_operand" "r")))]
  "TARGET_PLUS_DMPY"
  "vsub<V_suffix>\\t%0,0,%1"
  [(set_attr "length" "8")
   (set_attr "type" "multi")])

(define_insn "reduc_plus_scal_v4hi"
  [(set (match_operand:HI 0 "even_register_operand" "=r")
	(unspec:HI [(match_operand:V4HI 1 "even_register_operand" "r")]
		   UNSPEC_ARC_QMPYH))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_PLUS_QMACW"
  "qmpyh\\t%0,%1,1"
  [(set_attr "length" "4")
   (set_attr "type" "multi")])

(define_insn "reduc_plus_scal_v2si"
  [(set (match_operand:SI 0 "even_register_operand" "=r")
	(unspec:SI [(match_operand:V2SI 1 "even_register_operand" "r")]
		   UNSPEC_ARC_DMPYWH))
   (clobber (reg:DI ARCV2_ACC))]
  "TARGET_PLUS_DMPY"
  "dmpywh\\t%0,%1,1"
  [(set_attr "length" "4")
   (set_attr "type" "multi")])

(define_insn_and_split "vec_duplicatev2si"
  [(set (match_operand:V2SI 0 "register_operand" "=r")
	(vec_duplicate:V2SI
	 (match_operand:SI 1 "nonmemory_operand" "ri")))]
  ""
  "#"
  "reload_completed"
  [(const_int 0)]
  {
   rtx high_dest = gen_highpart (SImode, operands[0]);
   rtx low_dest = gen_lowpart (SImode, operands[0]);
   emit_move_insn (high_dest, operands[1]);
   emit_move_insn (low_dest, operands[1]);
   DONE;
  }
  [(set_attr "length" "8")
   (set_attr "type" "multi")])

(define_insn_and_split "vec_duplicatev4hi"
  [(set (match_operand:V4HI 0 "register_operand" "=r")
	(vec_duplicate:V4HI
	 (match_operand:HI 1 "nonmemory_operand" "ri")))]
  "TARGET_BARREL_SHIFTER"
  "#"
  "reload_completed"
  [(const_int 0)]
  {
   rtx high_dest = gen_highpart (SImode, operands[0]);
   rtx low_dest = gen_lowpart (SImode, operands[0]);
   rtx tmp = gen_lowpart (SImode, operands[1]);
   emit_insn (gen_rtx_SET (high_dest,
			   gen_rtx_ASHIFT (SImode, tmp, GEN_INT (16))));
   emit_insn (gen_rtx_SET (low_dest,
			   gen_rtx_IOR (SImode, high_dest, tmp)));
   emit_move_insn (high_dest, low_dest);
   DONE;
  }
  [(set_attr "length" "12")
   (set_attr "type" "multi")])
