/* PR c++/125081 */
/* { dg-do compile } */
/* { dg-options "-std=c23" { target c } } */
/* { dg-additional-options "-Wconstant-logical-operand" } */

void foo (int);
enum A { B, C };
enum D { E, F, G, H, I };

void
bar (int x, bool y)
{
  if (x && 64)	/* { dg-warning "use of logical '\\\&\\\&' with constant operand '64'" } */
    foo (1);	/* { dg-message "note: use '\\\&' for bitwise operation" "" { target *-*-* } .-1 } */
  if (128 && x)	/* { dg-warning "use of logical '\\\&\\\&' with constant operand '128'" } */
    foo (2);	/* { dg-message "note: use '\\\&' for bitwise operation" "" { target *-*-* } .-1 } */
  if (x && 1)
    foo (3);
  if (1 && x)
    foo (4);
  if (x && 0)
    foo (5);
  if (0 && x)
    foo (6);
  if (x || 32)	/* { dg-warning "use of logical '\\\|\\\|' with constant operand '32'" } */
    foo (7);	/* { dg-message "note: use '\\\|' for bitwise operation" "" { target *-*-* } .-1 } */
  if (256 || x)	/* { dg-warning "use of logical '\\\|\\\|' with constant operand '256'" } */
    foo (8);	/* { dg-message "note: use '\\\|' for bitwise operation" "" { target *-*-* } .-1 } */
  if (x || 1)
    foo (9);
  if (1 || x)
    foo (10);
  if (x || 0)
    foo (11);
  if (0 || x)
    foo (12);
  if (y && 64)
    foo (13);
  if (128 && y)
    foo (14);
  if (y || 32)
    foo (15);
  if (256 || y)
    foo (16);
  if (x && B)
    foo (17);
  if (B && x)
    foo (18);
  if (x && C)
    foo (19);
  if (C && x)
    foo (20);
  if (x && E)
    foo (21);
  if (E && x)
    foo (22);
  if (x && F)	/* { dg-warning "use of logical '\\\&\\\&' with constant operand '\[1F]'" } */
    foo (23);	/* { dg-message "note: use '\\\&' for bitwise operation" "" { target *-*-* } .-1 } */
  if (F && x)	/* { dg-warning "use of logical '\\\&\\\&' with constant operand '\[1F]'" } */
    foo (24);	/* { dg-message "note: use '\\\&' for bitwise operation" "" { target *-*-* } .-1 } */
  if (x && G)	/* { dg-warning "use of logical '\\\&\\\&' with constant operand '\[2G]'" } */
    foo (25);	/* { dg-message "note: use '\\\&' for bitwise operation" "" { target *-*-* } .-1 } */
  if (G && x)	/* { dg-warning "use of logical '\\\&\\\&' with constant operand '\[2G]'" } */
    foo (26);	/* { dg-message "note: use '\\\&' for bitwise operation" "" { target *-*-* } .-1 } */
  if (x || B)
    foo (27);
  if (B || x)
    foo (28);
  if (x || C)
    foo (29);
  if (C || x)
    foo (30);
  if (x || E)
    foo (31);
  if (E || x)
    foo (32);
  if (x || F)	/* { dg-warning "use of logical '\\\|\\\|' with constant operand '\[1F]'" } */
    foo (33);	/* { dg-message "note: use '\\\|' for bitwise operation" "" { target *-*-* } .-1 } */
  if (F || x)	/* { dg-warning "use of logical '\\\|\\\|' with constant operand '\[1F]'" } */
    foo (34);	/* { dg-message "note: use '\\\|' for bitwise operation" "" { target *-*-* } .-1 } */
  if (x || G)	/* { dg-warning "use of logical '\\\|\\\|' with constant operand '\[2G]'" } */
    foo (35);	/* { dg-message "note: use '\\\|' for bitwise operation" "" { target *-*-* } .-1 } */
  if (G || x)	/* { dg-warning "use of logical '\\\|\\\|' with constant operand '\[2G]'" } */
    foo (36);	/* { dg-message "note: use '\\\|' for bitwise operation" "" { target *-*-* } .-1 } */
}

void
baz (int x, bool y)
{
  if (x && 63 + 1)	/* { dg-warning "use of logical '\\\&\\\&' with constant operand '64'" } */
    foo (1);		/* { dg-message "note: use '\\\&' for bitwise operation" "" { target *-*-* } .-1 } */
  if (127 + 1 && x)	/* { dg-warning "use of logical '\\\&\\\&' with constant operand '128'" } */
    foo (2);		/* { dg-message "note: use '\\\&' for bitwise operation" "" { target *-*-* } .-1 } */
  if (x || 31 + 1)	/* { dg-warning "use of logical '\\\|\\\|' with constant operand '32'" } */
    foo (7);		/* { dg-message "note: use '\\\|' for bitwise operation" "" { target *-*-* } .-1 } */
  if (255 + 1 || x)	/* { dg-warning "use of logical '\\\|\\\|' with constant operand '256'" } */
    foo (8);		/* { dg-message "note: use '\\\|' for bitwise operation" "" { target *-*-* } .-1 } */
  if (y && 63 + 1)
    foo (13);
  if (127 + 1 && y)
    foo (14);
  if (y || 31 + 1)
    foo (15);
  if (255 + 1 || y)
    foo (16);
}
