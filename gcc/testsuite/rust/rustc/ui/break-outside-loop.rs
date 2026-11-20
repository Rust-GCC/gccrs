struct Foo {
    t: String
}

fn cond() -> bool { true }

fn foo<F>(_: F) where F: FnOnce() {}

fn main() {
    let pth = break; // { dg-error ".E0268." "" { target *-*-* } }
    if cond() { continue } // { dg-error ".E0268." "" { target *-*-* } }

    while cond() {
        if cond() { break }
        if cond() { continue }
        foo(|| {
            if cond() { break } // { dg-error ".E0267." "" { target *-*-* } }
            if cond() { continue } // { dg-error ".E0267." "" { target *-*-* } }
        })
    }

    let rs: Foo = Foo{t: pth};

    let unconstrained = break; // { dg-error ".E0268." "" { target *-*-* } }

    // This used to ICE because `target_id` passed to `check_expr_break` would be the closure and
    // not the `loop`, which failed in the call to `find_breakable`. (#65383)
    'lab: loop {
        || {
            break 'lab;
// { dg-error ".E0267." "" { target *-*-* } .-1 }
// { dg-error ".E0267." "" { target *-*-* } .-2 }
        };
    }
}

