extern {
    fn foo<T>(); // { dg-error ".E0044." "" { target *-*-* } }
}

fn main() {
    foo::<i32>();
}

