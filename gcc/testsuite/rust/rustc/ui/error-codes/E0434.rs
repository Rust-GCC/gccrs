fn foo() {
    let y = 5;
    fn bar() -> u32 {
        y // { dg-error ".E0434." "" { target *-*-* } }
    }
}

fn main () {
}

