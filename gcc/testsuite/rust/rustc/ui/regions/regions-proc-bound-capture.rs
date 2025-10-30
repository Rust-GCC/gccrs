fn borrowed_proc<'a>(x: &'a isize) -> Box<dyn FnMut()->(isize) + 'a> {
    // This is legal, because the region bound on `proc`
    // states that it captures `x`.
    Box::new(move|| { *x })
}

fn static_proc(x: &isize) -> Box<dyn FnMut() -> (isize) + 'static> {
    // This is illegal, because the region bound on `proc` is 'static.
    Box::new(move || { *x }) // { dg-error ".E0759." "" { target *-*-* } }
}

fn main() { }

