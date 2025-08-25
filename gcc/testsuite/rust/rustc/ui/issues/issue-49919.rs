fn foo<'a, T: 'a>(t: T) -> Box<dyn Fn() -> &'a T + 'a> {
    let foo: Box<dyn for <'c> Fn() -> &'c T> = Box::new(move || &t);
// { dg-error ".E0582." "" { target *-*-* } .-1 }
    unimplemented!()
}

fn main() {}

