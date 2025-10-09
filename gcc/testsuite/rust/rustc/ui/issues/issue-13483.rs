fn main() {
    if true {
    } else if { // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    } else {
    }
}

fn foo() {
    if true {
    } else if { // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    }
    bar();
}

fn bar() {}

