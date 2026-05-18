/* { dg-do compile { target { ! riscv_abi_e } } } */
/* { dg-skip-if "-march given" { *-*-* } { "-march=*" } } */
/* { dg-options "-mcpu=xt-c9501fdvt" { target { rv64 } } } */
/* XuanTie C950 => rv64imafdcv_zic64b_zicbom_zicbop_zicboz_ziccamoa_ziccif
               _zicclsm_ziccrse_zicfilp_zicfiss_zicntr_zicond_zicsr_zifencei
	       _zihintntl_zihintpause_zihpm_zimop_za64rs_zabha_zacas_zama16b
	       _zawrs_zfa_zfbfmin_zfh_zfhmin_zca_zcb_zcd_zcmop_zba_zbb_zbc_zbs
	       _zkr_zkt_zmmul_zvbb_zvbc_zvfbfmin_zvfbfwma_zvfh_zvfhmin_zvkb
	       _zvkg_zvkn_zvknc_zvkned_zvkng_zvknha_zvknhb_zvksc_zvksed_zvksg
	       _zvksh_zvkt_zvl256b_shcounterenw_shgatpa_shtvala_shvsatpa
	       _shvstvala_shvstvecd_smaia_smcntrpmf_smcsrind_smepmp_smmpm_smnpm
	       _smrnmi_ssaia_ssccptr_sscofpmf_sscounterenw_sscsrind_ssnpm_sspm
	       _ssstateen_ssstrict_sstc_sstvala_sstvecd_ssu64xl_supm_svade
	       _svadu_svbare_svinval_svnapot_svpbmt_svvptc */

#if !((__riscv_xlen == 64)		\
      && !defined(__riscv_32e)		\
      && (__riscv_flen == 64)		\
      && defined(__riscv_i)		\
      && defined(__riscv_m)		\
      && defined(__riscv_a)		\
      && defined(__riscv_f)		\
      && defined(__riscv_d)		\
      && defined(__riscv_c)		\
      && defined(__riscv_b)		\
      && defined(__riscv_v)		\
      && defined(__riscv_zic64b)	\
      && defined(__riscv_zicbom)	\
      && defined(__riscv_zicbop)	\
      && defined(__riscv_zicboz)	\
      && defined(__riscv_ziccamoa)	\
      && defined(__riscv_ziccif)	\
      && defined(__riscv_zicclsm)	\
      && defined(__riscv_ziccrse)	\
      && defined(__riscv_zicfilp)	\
      && defined(__riscv_zicfiss)	\
      && defined(__riscv_zicntr)	\
      && defined(__riscv_zicond)	\
      && defined(__riscv_zicsr)		\
      && defined(__riscv_zifencei)	\
      && defined(__riscv_zihintntl)	\
      && defined(__riscv_zihintpause)	\
      && defined(__riscv_zihpm)		\
      && defined(__riscv_zimop)		\
      && defined(__riscv_za64rs)	\
      && defined(__riscv_zabha)		\
      && defined(__riscv_zacas)		\
      && defined(__riscv_zama16b)	\
      && defined(__riscv_zawrs)		\
      && defined(__riscv_zfa)		\
      && defined(__riscv_zfbfmin)	\
      && defined(__riscv_zfh)		\
      && defined(__riscv_zfhmin)	\
      && defined(__riscv_zca)		\
      && defined(__riscv_zcb)		\
      && defined(__riscv_zcd)		\
      && defined(__riscv_zcmop)		\
      && defined(__riscv_zba)		\
      && defined(__riscv_zbb)		\
      && defined(__riscv_zbc)		\
      && defined(__riscv_zbs)		\
      && defined(__riscv_zkr)		\
      && defined(__riscv_zkt)		\
      && defined(__riscv_zmmul)		\
      && defined(__riscv_zvbb)		\
      && defined(__riscv_zvbc)		\
      && defined(__riscv_zvfbfmin)	\
      && defined(__riscv_zvfbfwma)	\
      && defined(__riscv_zvfh)		\
      && defined(__riscv_zvfhmin)	\
      && defined(__riscv_zvkb)		\
      && defined(__riscv_zvkg)		\
      && defined(__riscv_zvkn)		\
      && defined(__riscv_zvknc)		\
      && defined(__riscv_zvkned)	\
      && defined(__riscv_zvkng)		\
      && defined(__riscv_zvknha)	\
      && defined(__riscv_zvknhb)	\
      && defined(__riscv_zvksc)		\
      && defined(__riscv_zvksed)	\
      && defined(__riscv_zvksg)		\
      && defined(__riscv_zvksh)		\
      && defined(__riscv_zvkt)		\
      && defined(__riscv_zvl256b)	\
      && defined(__riscv_shcounterenw)	\
      && defined(__riscv_shgatpa)	\
      && defined(__riscv_shtvala)	\
      && defined(__riscv_shvsatpa)	\
      && defined(__riscv_shvstvala)	\
      && defined(__riscv_shvstvecd)	\
      && defined(__riscv_smaia)		\
      && defined(__riscv_smcntrpmf)	\
      && defined(__riscv_smcsrind)	\
      && defined(__riscv_smepmp)	\
      && defined(__riscv_smmpm)		\
      && defined(__riscv_smnpm)		\
      && defined(__riscv_smrnmi)	\
      && defined(__riscv_ssaia)		\
      && defined(__riscv_ssccptr)	\
      && defined(__riscv_sscofpmf)	\
      && defined(__riscv_sscounterenw)	\
      && defined(__riscv_sscsrind)	\
      && defined(__riscv_ssnpm)		\
      && defined(__riscv_sspm)		\
      && defined(__riscv_ssstateen)	\
      && defined(__riscv_ssstrict)	\
      && defined(__riscv_sstc)		\
      && defined(__riscv_sstvala)	\
      && defined(__riscv_sstvecd)	\
      && defined(__riscv_ssu64xl)	\
      && defined(__riscv_supm)		\
      && defined(__riscv_svade)		\
      && defined(__riscv_svadu)		\
      && defined(__riscv_svbare)	\
      && defined(__riscv_svinval)	\
      && defined(__riscv_svnapot)	\
      && defined(__riscv_svpbmt)	\
      && defined(__riscv_svvptc))
#error "unexpected arch"
#endif

int main()
{
  return 0;
}
