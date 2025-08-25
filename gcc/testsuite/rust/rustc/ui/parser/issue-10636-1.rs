struct Obj {
// { dg-note "" "" { target *-*-* } .-1 }
    member: usize
)
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }

fn main() {}

