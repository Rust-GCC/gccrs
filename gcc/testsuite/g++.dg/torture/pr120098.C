// { dg-do run }

#define N ((int*)0)
namespace {
class A {
public:
  unsigned length;
};
class B {};
class MultiTermDocs : public virtual B {
protected:
  A readerTermDocs;
  A subReaders;
  virtual B *m_fn1(int *) { return (B *)0; }
  virtual inline  ~MultiTermDocs();
  inline void wrap(void)
  {
  m_fn1(N);
  m_fn1(N);
  m_fn1(N);
  m_fn1(N);
  m_fn1(N);
  m_fn1(N);
  m_fn1(N);
  m_fn1(N);
  m_fn1(N);
  }
};
class C : MultiTermDocs {
  B *m_fn1(int *);
};
MultiTermDocs::~MultiTermDocs() {
  wrap ();
  if (&readerTermDocs) {
    B *a;
    for (unsigned i = 0; i < subReaders.length; i++)
      (a != 0);
  }
}

B *C::m_fn1(int *) { __builtin_abort (); }
}

int main()
{
  class C c;
}
