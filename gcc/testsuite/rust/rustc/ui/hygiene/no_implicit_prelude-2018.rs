// edition:2018

#[no_implicit_prelude]
mod bar {
    fn f() {
        ::std::print!(""); // OK
        print!(); // { dg-error "" "" { target *-*-* } }
    }
}

fn main() {}

