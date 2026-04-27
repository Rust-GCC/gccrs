/* PR target/124365 */
/* { dg-lto-do link } */
/* { dg-skip-if "" { ! aarch64*-*-linux* } } */
/* { dg-skip-if "" { aarch64_gas_has_build_attributes } } */
/* { dg-require-effective-target mbranch_protection_ok } */
/* { dg-lto-options { { -flto -O2 -mbranch-protection=none } } } */
/* { dg-extra-ld-options "-nostdlib -nostartfiles -Wl,-e,main" } */

int foo ();
int
main (void)
{
  return 0;
}

/* { dg-final { object-readelf-notes-not {AArch64 feature:\s+BTI\s+PAC\s+(GCS|<unknown:\s+[0-9]+>)} } } */
