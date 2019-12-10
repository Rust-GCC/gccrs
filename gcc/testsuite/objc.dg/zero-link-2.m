/* Check if the '-fno-zero-link' flag correctly _omits_ an objc_getClass() call. */
/* Contributed by Ziemowit Laski <zlaski@apple.com>.  */

/* { dg-do compile { target *-*-darwin* } } */
/* { dg-skip-if "" { *-*-* } { "-fgnu-runtime" } { "" } } */
/* { dg-options "-fno-zero-link" } */

#include "../objc-obj-c++-shared/F-NSObject.h"

extern void abort(void);
#define CHECK_IF(expr) if(!(expr)) abort();

@interface Base: NSObject
+ (int) getValue;
@end

@implementation Base
+ (int) getValue { return 1593; }
@end

int main(void) {
  int val = [Base getValue];
  CHECK_IF(val == 1593);
  return 0;
}

/* { dg-final { scan-assembler "_OBJC_ClassRefs_0"  { target { *-*-darwin* && { ! lp64 } } } } } */
/* { dg-final { scan-assembler "_OBJC_ClassRef_Base"  { target { *-*-darwin* && { lp64 } } } } } */
/* { dg-final { scan-assembler-not "objc_getClass" } } */
