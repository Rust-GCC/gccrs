// a good test that we merge paths correctly in the presence of a
// variable that's used before it's declared

fn my_panic() -> ! { panic!(); }

fn main() {
    match true { false => { my_panic(); } true => { } }

    println!("{}", x); // { dg-error ".E0425." "" { target *-*-* } }
    let x: isize;
}

