#![feature(no_core)]
#![no_core]

enum LocalType {
    X  = (1/0) // { dg-error "division by zero" "" { target *-*-* } }
               // { dg-error "is not a constant expression" "" { target *-*-* } .-1 }
}