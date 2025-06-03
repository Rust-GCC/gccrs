/* { dg-options "-std=gnu17 -fpermissive -Wno-implicit-function-declaration -Wno-implicit-int -Wno-int-conversion -Wno-incompatible-pointer-types -Wno-return-mismatch -Wno-declaration-missing-parameter-type" } */

/* This test checks that permerrors can be disabled using -Wno-* options even
   if -fpermissive is used.  */

#include "permerror-default.c"
