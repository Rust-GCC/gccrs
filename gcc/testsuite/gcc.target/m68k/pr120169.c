/* { dg-do compile } */
/* { dg-options "-O2 -g -fPIC" } */

typedef void *decl_node;
bool decl_isRecordField(decl_node);
void decl_lookupSym(int);
void mcDebug_assert(bool);
typedef enum { decl_varient } decl_nodeT;
typedef struct decl_nodeRec_r *decl_node__opaque;
struct decl_nodeRec_r {
  decl_nodeT kind;
};
decl_node__opaque tag, putVarientTag_tag, putVarientTag_v,
    decl_buildVarientSelector_f;
int decl_buildVarientSelector_tag;
decl_node decl_buildVarientSelector_type;
void decl_buildVarientSelector() {
  mcDebug_assert(0);
  if (decl_buildVarientSelector_type) {
    decl_lookupSym(decl_buildVarientSelector_tag);
    mcDebug_assert(0);
    switch (putVarientTag_v->kind)
    case decl_varient:
      tag = putVarientTag_tag;
  } else {
    bool __trans_tmp_2 = decl_isRecordField(decl_buildVarientSelector_f);
    mcDebug_assert(__trans_tmp_2);
    mcDebug_assert(0);
    switch (putVarientTag_v->kind)
    case decl_varient:
      tag = putVarientTag_tag;
  }
}
