/* { dg-do compile } */
/* { dg-skip-if "-march given" { *-*-* } { "-march=*" } } */
/* { dg-options "-mcpu=spacemit-x100" { target { rv64 } } } */
/* Spacemit X100 => rva23s64_smepmp_smnpm_smstateen_sspm_zbc_zbkc_
 * zfbfmin_zfh_zvbc_zvfbfwma_zvfh_zvknc_zvkng_zvknha_zvksc_zvksg_
 * zvl256b_xsmtvdot
 */

#if !((__riscv_xlen == 64)		\
      && !defined(__riscv_32e)		\
      && defined(__riscv_mul)		\
      && defined(__riscv_atomic)	\
      && (__riscv_flen == 64)		\
      && defined(__riscv_compressed)	\
      && defined(__riscv_v)		\
      && defined(__riscv_h)		\
      && defined(__riscv_zic64b)	\
      && defined(__riscv_zba)		\
      && defined(__riscv_zbb)		\
      && defined(__riscv_zbc)		\
      && defined(__riscv_zbs)		\
      && defined(__riscv_zfh)		\
      && defined(__riscv_zfhmin)	\
      && defined(__riscv_ziccamoa)	\
      && defined(__riscv_ziccif)	\
      && defined(__riscv_zicclsm)	\
      && defined(__riscv_ziccrse)	\
      && defined(__riscv_zicsr)		\
      && defined(__riscv_zifencei)	\
      && defined(__riscv_zihintntl)	\
      && defined(__riscv_svpbmt)	\
      && defined(__riscv_smepmp)	\
      && defined(__riscv_sstc)		\
      && defined(__riscv_sscofpmf)	\
      && defined(__riscv_zicond)	\
      && defined(__riscv_zicboz)	\
      && defined(__riscv_zicbom)	\
      && defined(__riscv_zicbop)	\
      && defined(__riscv_zicntr)	\
      && defined(__riscv_zihpm)		\
      && defined(__riscv_za64rs)	\
      && defined(__riscv_zkt)		\
      && defined(__riscv_svinval)	\
      && defined(__riscv_svnapot)	\
      && defined(__riscv_zihintpause)	\
      && defined(__riscv_zbkc)		\
      && defined(__riscv_zvfh)		\
      && defined(__riscv_zvfhmin)	\
      && defined(__riscv_zvl256b)	\
      && defined(__riscv_zfa)		\
      && defined(__riscv_zca)		\
      && defined(__riscv_zcb)		\
      && defined(__riscv_zcd)		\
      && defined(__riscv_zvbb)		\
      && defined(__riscv_zvkng)		\
      && defined(__riscv_zvknha)	\
      && defined(__riscv_smstateen)	\
      && defined(__riscv_ssstateen)	\
      && defined(__riscv_zfbfmin)	\
      && defined(__riscv_zvfbfmin)	\
      && defined(__riscv_zvfbfwma)	\
      && defined(__riscv_zimop)		\
      && defined(__riscv_zcmop)		\
      && defined(__riscv_smnpm)		\
      && defined(__riscv_ssnpm)		\
      && defined(__riscv_sspm)		\
      && defined(__riscv_zawrs)		\
      && defined(__riscv_zvknc)		\
      && defined(__riscv_zvksc)		\
      && defined(__riscv_zvksg)		\
      && defined(__riscv_supm)		\
      && defined(__riscv_svbare)	\
      && defined(__riscv_svade)		\
      && defined(__riscv_ssccptr)	\
      && defined(__riscv_sstvecd)	\
      && defined(__riscv_sstvala)	\
      && defined(__riscv_sscounterenw)	\
      && defined(__riscv_ssu64xl)	\
      && defined(__riscv_sha)		\
      && defined(__riscv_xsmtvdot))
#error "unexpected arch"
#endif

int main()
{
  return 0;
}
