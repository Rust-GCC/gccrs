// { dg-additional-options "-frust-unused-check-2.0" }
#![feature(no_core)]
#![no_core]

pub struct Point {
    pub x: i32,
    pub y: i32,
}

pub fn foo(p: Point) -> i32 {
    match p {
	Point { x: x, y: _y } => x,
// { dg-warning "in this pattern is redundant" "" { target *-*-* } .-1 }
    }
}
