/* { dg-do compile } */
/* { dg-options "-std=c23" } */

struct s { void (*p)(const struct s*); };
struct s { void (*p)(const struct s*); };


