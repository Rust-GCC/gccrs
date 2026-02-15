// compile-flags: --edition 2018

#![feature(try_blocks)]

pub fn main() {
    let res: Result<u32, i32> = try {
        Err("")?; // { dg-error ".E0277." "" { target *-*-* } }
        5
    };

    let res: Result<i32, i32> = try {
        "" // { dg-error ".E0271." "" { target *-*-* } }
    };

    let res: Result<i32, i32> = try { }; // { dg-error ".E0271." "" { target *-*-* } }

    let res: () = try { };
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }

    let res: i32 = try { 5 }; // { dg-error ".E0277." "" { target *-*-* } }
}

