fn main() {
    let bar: fn(&mut u32) = |_| {};

    fn foo(x: Box<dyn Fn(&i32)>) {}
    let bar = Box::new(|x: &i32| {}) as Box<dyn Fn(_)>;
    foo(bar); // { dg-error ".E0308." "" { target *-*-* } }
}

