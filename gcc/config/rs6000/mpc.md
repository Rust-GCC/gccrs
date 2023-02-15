;; Scheduling description for Motorola PowerPC processor cores.
;;   Copyright (C) 2003-2023 Free Software Foundation, Inc.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.
;;
;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

(define_automaton "mpc,mpcfp")
(define_cpu_unit "iu_mpc,mciu_mpc" "mpc")
(define_cpu_unit "fpu_mpc" "mpcfp")
(define_cpu_unit "lsu_mpc,bpu_mpc" "mpc")

;; MPCCORE 32-bit SCIU, MCIU, LSU, FPU, BPU
;; 505/801/821/823

(define_insn_reservation "mpccore-load" 2
  (and (eq_attr "type" "load,load_l,store_c,sync")
       (eq_attr "cpu" "mpccore"))
  "lsu_mpc")

(define_insn_reservation "mpccore-store" 2
  (and (eq_attr "type" "store,fpstore")
       (eq_attr "cpu" "mpccore"))
  "lsu_mpc")

(define_insn_reservation "mpccore-fpload" 2
  (and (eq_attr "type" "fpload")
       (eq_attr "cpu" "mpccore"))
  "lsu_mpc")

(define_insn_reservation "mpccore-integer" 1
  (and (ior (eq_attr "type" "integer,insert,trap,cntlz,isel")
	    (and (eq_attr "type" "add,logical,shift,exts")
		 (eq_attr "dot" "no")))
       (eq_attr "cpu" "mpccore"))
  "iu_mpc")

(define_insn_reservation "mpccore-two" 1
  (and (eq_attr "type" "two")
       (eq_attr "cpu" "mpccore"))
  "iu_mpc,iu_mpc")

(define_insn_reservation "mpccore-three" 1
  (and (eq_attr "type" "three")
       (eq_attr "cpu" "mpccore"))
  "iu_mpc,iu_mpc,iu_mpc")

(define_insn_reservation "mpccore-imul" 2
  (and (eq_attr "type" "mul")
       (eq_attr "cpu" "mpccore"))
  "mciu_mpc")

; Divide latency varies greatly from 2-11, use 6 as average
(define_insn_reservation "mpccore-idiv" 6
  (and (eq_attr "type" "div")
       (eq_attr "cpu" "mpccore"))
  "mciu_mpc*6")

(define_insn_reservation "mpccore-compare" 3
  (and (ior (eq_attr "type" "cmp")
	    (and (eq_attr "type" "add,logical,shift,exts")
		 (eq_attr "dot" "yes")))
       (eq_attr "cpu" "mpccore"))
  "iu_mpc,nothing,bpu_mpc")

(define_insn_reservation "mpccore-fpcompare" 2
  (and (eq_attr "type" "fpcompare")
       (eq_attr "cpu" "mpccore"))
  "fpu_mpc,bpu_mpc")

(define_insn_reservation "mpccore-fp" 4
  (and (eq_attr "type" "fp,fpsimple")
       (eq_attr "cpu" "mpccore"))
  "fpu_mpc*2")

(define_insn_reservation "mpccore-dmul" 5
  (and (eq_attr "type" "dmul")
       (eq_attr "cpu" "mpccore"))
  "fpu_mpc*5")

(define_insn_reservation "mpccore-sdiv" 10
  (and (eq_attr "type" "sdiv")
       (eq_attr "cpu" "mpccore"))
  "fpu_mpc*10")

(define_insn_reservation "mpccore-ddiv" 17
  (and (eq_attr "type" "ddiv")
       (eq_attr "cpu" "mpccore"))
  "fpu_mpc*17")

(define_insn_reservation "mpccore-mtjmpr" 4
  (and (eq_attr "type" "mtjmpr,mfjmpr")
       (eq_attr "cpu" "mpccore"))
  "bpu_mpc")

(define_insn_reservation "mpccore-jmpreg" 1
  (and (eq_attr "type" "jmpreg,branch,cr_logical,mfcr,mtcr,isync")
       (eq_attr "cpu" "mpccore"))
  "bpu_mpc")

