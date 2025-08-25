fn foo() -> u32 {
    return 'label: loop { break 'label 42; };
}

fn bar() -> u32 {
    loop { break 'label: loop { break 'label 42; }; }
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

pub fn main() {
    foo();
}

