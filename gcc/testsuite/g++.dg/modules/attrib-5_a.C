// PR c++/122786
// { dg-additional-options "-fmodules" }
// { dg-require-effective-target named_sections }
// { dg-module-cmi M }

export module M;

export inline void a() __attribute__((section("barbar")));
inline void a() {}

export [[gnu::section("quxqux")]] inline void b();
inline void b() {}
