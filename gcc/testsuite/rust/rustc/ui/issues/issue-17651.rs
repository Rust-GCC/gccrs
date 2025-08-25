// Test that moves of unsized values within closures are caught
// and rejected.

fn main() {
    (|| Box::new(*(&[0][..])))();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
}

