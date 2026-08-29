fn f<'r>(p: &'r mut fn(p: &mut ())) {
    (*p)(()) // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn main() {}

