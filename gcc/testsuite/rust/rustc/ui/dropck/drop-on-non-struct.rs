impl<'a> Drop for &'a mut isize {
// { dg-error ".E0117." "" { target *-*-* } .-1 }
// { dg-error ".E0117." "" { target *-*-* } .-2 }
    fn drop(&mut self) {
        println!("kaboom");
    }
}

impl Drop for Nonexistent {
// { dg-error ".E0412." "" { target *-*-* } .-1 }
    fn drop(&mut self) { }
}

fn main() {
}

