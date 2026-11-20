// compile-flags: -Zsave-analysis
// Also regression test for #69416

mod my_mod {
    pub struct MyStruct {
        priv_field: isize
    }
    pub fn MyStruct () -> MyStruct {
        MyStruct {priv_field: 4}
    }
    impl MyStruct {
        fn happyfun(&self) {}
    }
}

fn main() {
    let my_struct = my_mod::MyStruct();
    let _woohoo = (&my_struct).priv_field;
// { dg-error ".E0616." "" { target *-*-* } .-1 }

    let _woohoo = (Box::new(my_struct)).priv_field;
// { dg-error ".E0616." "" { target *-*-* } .-1 }

    (&my_struct).happyfun();               // { dg-error ".E0624." "" { target *-*-* } }

    (Box::new(my_struct)).happyfun();          // { dg-error ".E0624." "" { target *-*-* } }
    let nope = my_struct.priv_field;
// { dg-error ".E0616." "" { target *-*-* } .-1 }
}

