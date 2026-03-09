// { dg-do compile }
// { dg-require-effective-target fpic }
// { dg-additional-options "-fPIC" }

inline int &GetErrorRef() {
  thread_local int err;
  return err;
}
int SetError_err;
bool isREG___trans_tmp_4;
struct Operand {
  int : 6;
  int kind_ : 10;
  int bit_ : 14;
protected:
  int : 11;
public:
  bool isREG(int bit) {
    isREG___trans_tmp_4 = kind_ && bit_ & bit;
    return isREG___trans_tmp_4;
  }
  int getBit() { return bit_; }
};
struct Reg : Operand {
} index_, fp8_to_f16_upconvert_to_vnni_reg_data;
enum { i32e = 4 };
struct RegExp {
  RegExp(Reg &r) {
    if (r.isREG(i32e))
      GetErrorRef() = SetError_err;
    if (r.getBit())
      index_ = r;
  }
};
struct X {
  void operator[](RegExp);
} ptr;
void jit_brgemm_amx_uker_base_tfp8_to_f16_upconvert_to_vnni(Reg reg_buf) {
  RegExp __trans_tmp_3 = fp8_to_f16_upconvert_to_vnni_reg_data;
  ptr[reg_buf];
}
