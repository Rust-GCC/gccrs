fn foo<F: FnMut()>() {
    let _: Box<F> = Box::new(|| ());
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn main() {}

