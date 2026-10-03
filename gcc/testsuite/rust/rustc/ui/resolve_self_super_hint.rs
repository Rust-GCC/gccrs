mod a {
    extern crate alloc;
    use alloc::HashMap;
// { dg-error ".E0432." "" { target *-*-* } .-1 }
// { help ".E0432." "" { target *-*-* } .-2 }
// { suggestion ".E0432." "" { target *-*-* } .-3 }
    mod b {
        use alloc::HashMap;
// { dg-error ".E0432." "" { target *-*-* } .-1 }
// { help ".E0432." "" { target *-*-* } .-2 }
// { suggestion ".E0432." "" { target *-*-* } .-3 }
        mod c {
            use alloc::HashMap;
// { dg-error ".E0432." "" { target *-*-* } .-1 }
// { help ".E0432." "" { target *-*-* } .-2 }
// { suggestion ".E0432." "" { target *-*-* } .-3 }
            mod d {
                use alloc::HashMap;
// { dg-error ".E0432." "" { target *-*-* } .-1 }
// { help ".E0432." "" { target *-*-* } .-2 }
// { suggestion ".E0432." "" { target *-*-* } .-3 }
            }
        }
    }
}

fn main() {}

