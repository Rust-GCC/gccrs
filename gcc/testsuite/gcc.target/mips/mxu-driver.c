/* { dg-do assemble } */
/* { dg-additional-options "-mmxu -###" } */

int
main (void)
{
  return 0;
}

/* { dg-message ".*/.*as .* -mmxu" "assembler options" { target *-*-* } 0 } */
/* { dg-purne-output ".*" } */
