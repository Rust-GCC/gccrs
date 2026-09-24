fn main() {
    let foo = &mut 1;

    let &mut x = foo;
    x += 1; // { dg-error ".E0384." "" { target *-*-* } }

    // explicitly mut-ify internals
    let &mut mut x = foo;
    x += 1;

    // check borrowing is detected successfully
    let &mut ref x = foo;
    *foo += 1; // { dg-error ".E0506." "" { target *-*-* } }
    drop(x);
}

