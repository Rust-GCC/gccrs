struct S {}

fn foo(ctx: &mut S) -> String { // { dg-error ".E0308." "" { target *-*-* } }
    // Don't suggest to remove semicolon as it won't fix anything
    ctx.sleep = 0;
// { dg-error ".E0609." "" { target *-*-* } .-1 }
}

fn main() {}

