fn main() {
    let foo = Some(0);
    let bar = None;
    if Some(x) = foo {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    if Some(foo) = bar {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    if 3 = foo {} // { dg-error ".E0308." "" { target *-*-* } }
    if Some(3) = foo {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
}

