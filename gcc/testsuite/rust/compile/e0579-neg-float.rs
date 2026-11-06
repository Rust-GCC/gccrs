#![feature(no_core)]
#![no_core]

#![feature(exclusive_range_pattern)]

fn main() {
    let x = 1.0;

    match x {
        -1.2f32..-1.0f32 => 2,
    };
}
