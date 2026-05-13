// PR c++/123609
// { dg-do compile { target c++26 } }

struct [[=]] S {};			// { dg-error "expected primary-expression before ']' token" }
[[=]] int a;				// { dg-error "expected primary-expression before ']' token" }
