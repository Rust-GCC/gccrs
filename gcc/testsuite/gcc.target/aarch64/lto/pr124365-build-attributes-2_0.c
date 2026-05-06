/* PR target/124365 */
/* { dg-lto-do link } */
/* { dg-skip-if "" { ! aarch64*-*-linux* } } */
/* { dg-require-effective-target aarch64_gas_has_build_attributes } */
/* { dg-require-effective-target mbranch_protection_ok } */
/* { dg-lto-options { { -flto -O2 -mbranch-protection=none } } } */
/* { dg-extra-ld-options "-nostdlib -nostartfiles -Wl,-e,main" } */

int foo ();
int
main (void)
{
  return foo ();
}

/* { dg-final { object-readelf-attributes {Tag_Feature_BTI:\s+0\s+\(0x0\)} } } */
/* { dg-final { object-readelf-attributes {Tag_Feature_PAC:\s+0\s+\(0x0\)} } } */
/* { dg-final { object-readelf-attributes {Tag_Feature_GCS:\s+0\s+\(0x0\)} } } */
