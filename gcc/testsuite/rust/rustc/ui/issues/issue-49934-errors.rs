fn foo<#[derive(Debug)] T>() {
// { dg-error ".E0774." "" { target *-*-* } .-1 }
// { dg-error ".E0774." "" { target *-*-* } .-2 }
    match 0 {
        #[derive(Debug)]
// { dg-error ".E0774." "" { target *-*-* } .-1 }
// { dg-error ".E0774." "" { target *-*-* } .-2 }
        _ => (),
    }
}

fn main() {
}

