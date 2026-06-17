/* { dg-do compile } */
/* { dg-options "-O2 -Wno-stringop-overread -std=c++11" } */

int *__distance___last;
long move___trans_tmp_9;
void move(int *__first, int *__result) {
  move___trans_tmp_9 = __distance___last - __first;
  if (__builtin_expect(move___trans_tmp_9, true))
    __builtin_memmove(__result, __first, move___trans_tmp_9);
}
struct SmallVectorTemplateCommon {
  void operator[](long);
};
struct SmallVectorTemplateBase : SmallVectorTemplateCommon {};
void erase(int *CI) { move(CI + 1, CI); }
struct : SmallVectorTemplateBase {
} FindInOperandList_Ops;
int Op, ReassociatePassOptimizeAdd___trans_tmp_1;
unsigned FindInOperandList(unsigned i) {
  for (unsigned j = i + 1;;)
    if (FindInOperandList_Ops[j], Op)
      return j;
}
void ReassociatePassOptimizeAdd() {
  for (unsigned i;; ++i) {
    FindInOperandList(i);
    erase(&ReassociatePassOptimizeAdd___trans_tmp_1 + i);
  }
}

