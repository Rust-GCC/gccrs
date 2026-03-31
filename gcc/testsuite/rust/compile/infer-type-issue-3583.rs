#![feature(no_core)]
#![no_core]

struct Test10 {
    a: _, // { dg-error "the placeholder ... is not allowed within types on item" }
}

struct Test11 {
    a: _, // { dg-error "the placeholder ... is not allowed within types on item" }
    b: i32,
}

struct Test12 {
    a: (_, _), // { dg-error "the placeholder ... is not allowed within types on item" }
}

struct Test13 {
    a: [_; _], // { dg-error "the placeholder ... is not allowed within types on item" }
}

struct Test14 {
    a: [i32; _], // { dg-error "the placeholder ... is not allowed within types on item" }
}
