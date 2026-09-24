// revisions: migrate nll
//[nll] compile-flags:-Zborrowck=mir

fn static_id<'a,'b>(t: &'a ()) -> &'static ()
    where 'a: 'static { t }
fn static_id_indirect<'a,'b>(t: &'a ()) -> &'static ()
    where 'a: 'b, 'b: 'static { t }
fn static_id_wrong_way<'a>(t: &'a ()) -> &'static () where 'static: 'a {
    t // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}

fn error(u: &(), v: &()) {
    static_id(&u); // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
    static_id_indirect(&v); // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
}

fn main() {}

