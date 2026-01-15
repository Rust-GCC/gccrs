// { dg-additional-options "-frust-unused-check-2.0" }

struct my_struct; // { dg-warning "struct is never constructed: .my.struct." }
// { dg-warning "struct .my.struct. should have an upper camel case name" "" { target *-*-* } .-1 }

enum my_enum {}
// { dg-warning "enum .my.enum. should have an upper camel case name" "" { target *-*-* } .-1 }

trait my_trait {}
// { dg-warning "trait .my.trait. should have an upper camel case name" "" { target *-*-* } .-1 }

