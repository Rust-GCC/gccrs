trait x {
    fn use_x<T>(&self);
}
struct y(());
impl x for y {
    fn use_x<T>(&self) {
        struct foo { // { dg-error "" "" { target *-*-* } }
            i: ()
        }
        fn new_foo<T>(i: ()) -> foo {
            foo { i: i }
        }
    }
}

