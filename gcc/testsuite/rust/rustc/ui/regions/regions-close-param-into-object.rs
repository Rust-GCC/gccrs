trait X { fn foo(&self) {} }

fn p1<T>(v: T) -> Box<dyn X + 'static>
    where T : X
{
    Box::new(v) // { dg-error ".E0310." "" { target *-*-* } }
}

fn p2<T>(v: Box<T>) -> Box<dyn X + 'static>
    where Box<T> : X
{
    Box::new(v) // { dg-error ".E0310." "" { target *-*-* } }
}

fn p3<'a,T>(v: T) -> Box<dyn X + 'a>
    where T : X
{
    Box::new(v) // { dg-error ".E0309." "" { target *-*-* } }
}

fn p4<'a,T>(v: Box<T>) -> Box<dyn X + 'a>
    where Box<T> : X
{
    Box::new(v) // { dg-error ".E0309." "" { target *-*-* } }
}

fn main() {}

