// PR c++/38021
// { dg-do compile { target c++11 } }

enum : { };	// { dg-error "expected|ISO" }
enum : 3 { };	// { dg-error "expected" }
