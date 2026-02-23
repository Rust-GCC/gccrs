/* { dg-do compile } */
/* { dg-require-ifunc "" } */
/* { dg-additional-options "-flto" } */
class foo {
public:
    virtual void bar () = 0;
};

class baz : public foo {
public:
    void bar ();
};

__attribute__((target_clones("default", "flagm")))
void baz::bar () {
} /* { dg-message "sorry, unimplemented: virtual function multiversioning not supported" } */
