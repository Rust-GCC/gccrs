/* { dg-do assemble } */
/* { dg-additional-options "-mno-mxu -###" } */

int
main (void)
{
  return 0;
}

/* { dg-message ".*/.*as .* -mno-mxu" "assembler options" { target *-*-* } 0 } */
/* { dg-purne-output ".*" } */
