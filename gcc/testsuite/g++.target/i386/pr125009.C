/* { dg-do compile } */
/* { dg-options "-mtune=generic -O2 -std=c++11" } */
/* { dg-additional-options "-march=pentiumpro" { target ia32 } } */

void *operator new(__SIZE_TYPE__, void *);
template <typename Value> struct hash_map {
  void put(Value v) {
    int e;
    new (&e) Value(v);
  }
};
struct tree_string {
  char str[];
} string;
long long minsize;
struct attr_access {
  bool internal_p;
  bool static_p;
} __trans_tmp_1;
hash_map<attr_access> init_attr_rdwr_indices_rwm;
void init_attr_rdwr_indices() {
  for (;;) {
    attr_access acc{};
    if (*string.str) {
      acc.internal_p = true;
      acc.static_p = minsize;
    }
    __trans_tmp_1 = acc;
    init_attr_rdwr_indices_rwm.put(acc);
  }
}
