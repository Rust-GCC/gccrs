fn foo<T: 'static>() { }

fn boo<'a>() {
    return;

    let x = foo::<&'a u32>();
// { dg-error ".E0477." "" { target *-*-* } .-1 }
}

fn main() {}

