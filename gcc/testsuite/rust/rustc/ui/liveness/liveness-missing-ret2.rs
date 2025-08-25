fn f() -> isize { // { dg-error ".E0308." "" { target *-*-* } }
    // Make sure typestate doesn't interpret this match expression as
    // the function result
   match true { true => { } _ => {} };
}

fn main() { }

