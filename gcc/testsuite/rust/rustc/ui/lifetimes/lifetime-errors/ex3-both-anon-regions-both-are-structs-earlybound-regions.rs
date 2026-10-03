struct Ref<'a> {
    x: &'a u32,
}

fn foo<'a, 'b>(mut x: Vec<Ref<'a>>, y: Ref<'b>)
    where &'a (): Sized,
          &'b u32: Sized
{
    x.push(y); // { dg-error ".E0623." "" { target *-*-* } }
}

fn main() {}

