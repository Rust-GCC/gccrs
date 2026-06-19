// macro f should not be able to inject a reference to 'n'.

macro_rules! f { () => (n) }
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { dg-error ".E0425." "" { target *-*-* } .-2 }
// { dg-error ".E0425." "" { target *-*-* } .-3 }
// { dg-error ".E0425." "" { target *-*-* } .-4 }

fn main() -> (){
    for n in 0..1 {
        println!("{}", f!());
    }

    if let Some(n) = None {
        println!("{}", f!());
    }

    if false {
    } else if let Some(n) = None {
        println!("{}", f!());
    }

    while let Some(n) = None {
        println!("{}", f!());
    }
}

