trait A {}

impl A .. {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }

impl A      usize {}
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {}

