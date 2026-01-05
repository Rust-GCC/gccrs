extern GPF SUFFIX(trunc) (GPF);
extern GPF SUFFIX(ceil) (GPF);
extern GPF SUFFIX(floor) (GPF);
extern GPF SUFFIX(round) (GPF);

#define FORCE_FP_REG(a) asm volatile("fmov %d0, %d1" : "=w"(a) : "w"(a) :)

GPI test1a (GPF x) {
  FORCE_FP_REG(x);
  GPI res = SUFFIX(__builtin_trunc)(x);
  FORCE_FP_REG(res);
  return res;
}

GPI test1b (GPF x)
{
  FORCE_FP_REG(x);
  GPI res = SUFFIX(trunc)(x);
  FORCE_FP_REG(res);
  return res;
}

GPI test2a (GPF x)
{
  FORCE_FP_REG(x);
  GPI res = SUFFIX(__builtin_lceil)(x);
  FORCE_FP_REG(res);
  return res;
}

GPI test2b (GPF x)
{
  FORCE_FP_REG(x);
  GPI res = SUFFIX(ceil)(x);
  FORCE_FP_REG(res);
  return res;
}

GPI test2c (GPF x)
{
  FORCE_FP_REG(x);
  GPI res = SUFFIX(__builtin_ceil)(x);
  FORCE_FP_REG(res);
  return res;
}

GPI test3a (GPF x)
{
  FORCE_FP_REG(x);
  GPI res = SUFFIX(__builtin_lfloor)(x);
  FORCE_FP_REG(res);
  return res;
}

GPI test3b (GPF x)
{
  FORCE_FP_REG(x);
  GPI res = SUFFIX(floor)(x);
  FORCE_FP_REG(res);
  return res;
}

GPI test3c (GPF x)
{
  FORCE_FP_REG(x);
  GPI res = SUFFIX(__builtin_floor)(x);
  FORCE_FP_REG(res);
  return res;
}

GPI test4a (GPF x)
{
  FORCE_FP_REG(x);
  GPI res = SUFFIX(__builtin_round)(x);
  FORCE_FP_REG(res);
  return res;
}

GPI test4b (GPF x)
{
  FORCE_FP_REG(x);
  GPI res = SUFFIX(round)(x);
  FORCE_FP_REG(res);
  return res;
}


