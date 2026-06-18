/* { dg-do compile } */
/* { dg-options "-march=rv64gcbv_zicond -mabi=lp64d" { target rv64 } } */
/* { dg-options "-march=rv32gcbv_zicond -mabi=ilp32" { target rv32 } } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-Og"} } */

enum machine_mode
{
  VOIDmode,
  MAX_MACHINE_MODE,
  NUM_MACHINE_MODES = MAX_MACHINE_MODE
};
extern unsigned char mode_size[NUM_MACHINE_MODES];
void oof (enum machine_mode);
void
init_emit_once (enum machine_mode double_mode, enum machine_mode mode)
{
      if (((unsigned short) (((unsigned short) mode_size[mode]) * 8)) == 64
          && double_mode == VOIDmode)
        double_mode = mode;
  oof (double_mode);
}
/* { dg-final { scan-assembler-not "addi\t\[a-x0-9\]+,\[a-x0-9\]+,-64" } } */
/* { dg-final { scan-assembler "addi\t\[a-x0-9\]+,\[a-x0-9\]+,-8" } } */

