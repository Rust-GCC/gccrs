// run-rustfix

fn foo() -> i32 {
    0
}

fn main() {
    let _x: i32 = {
// { dg-error ".E0308." "" { target *-*-* } .-1 }
        foo(); // { help "" "" { target *-*-* } }
    };
}

