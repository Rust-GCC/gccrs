/* { dg-do compile } */
/* { dg-options "-std=c99 -g -finput-charset=latin1" } */
/* { dg-final { scan-file ucnid-16-utf8.s "²" } } */

/* This superscript is encoded in latin1; verify that we still get UTF-8 in the output.  */
int x� = 9;
