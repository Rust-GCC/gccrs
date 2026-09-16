// compile-flags: --edition 2018

#![feature(try_blocks)]

#[inline(never)]
fn do_something_with<T>(_x: T) {}

// This test checks that borrows made and returned inside try blocks are properly constrained
pub fn main() {
    {
        // Test that borrows returned from a try block must be valid for the lifetime of the
        // result variable
        let result: Result<(), &str> = try {
            let my_string = String::from("");
            let my_str: & str = & my_string;
// { dg-error ".E0597." "" { target *-*-* } .-1 }
            Err(my_str) ?;
            Err("") ?;
        };
        do_something_with(result);
    }

    {
        // Test that borrows returned from try blocks freeze their referent
        let mut i = 5;
        let k = &mut i;
        let mut j: Result<(), &mut i32> = try {
            Err(k) ?;
            i = 10; // { dg-error ".E0506." "" { target *-*-* } }
        };
        ::std::mem::drop(k); // { dg-error ".E0382." "" { target *-*-* } }
        i = 40; // { dg-error ".E0506." "" { target *-*-* } }

        let i_ptr = if let Err(i_ptr) = j { i_ptr } else { panic ! ("") };
        *i_ptr = 50;
    }
}

