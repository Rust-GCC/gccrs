// run-rustfix
#![allow(dead_code)]

mod foo {
    trait OtherTrait<'a> {}
    impl<'a> OtherTrait<'a> for &'a () {}

    trait ObjectTrait<T> {}
    trait MyTrait<T> {
        fn use_self<K>(&self) -> &();
    }
    trait Irrelevant {}

    impl<T> MyTrait<T> for dyn ObjectTrait<T> {
        fn use_self<K>(&self) -> &() { panic!() }
    }
    impl<T> Irrelevant for dyn ObjectTrait<T> {}

    fn use_it<'a, T>(val: &'a dyn ObjectTrait<T>) -> impl OtherTrait<'a> + 'a {
        val.use_self::<T>() // { dg-error ".E0759." "" { target *-*-* } }
    }
}

mod bar {
    trait ObjectTrait {}
    trait MyTrait {
        fn use_self(&self) -> &();
    }
    trait Irrelevant {}

    impl MyTrait for dyn ObjectTrait {
        fn use_self(&self) -> &() { panic!() }
    }
    impl Irrelevant for dyn ObjectTrait {}

    fn use_it<'a>(val: &'a dyn ObjectTrait) -> &'a () {
        val.use_self() // { dg-error ".E0772." "" { target *-*-* } }
    }
}

mod baz {
    trait ObjectTrait {}
    trait MyTrait {
        fn use_self(&self) -> &();
    }
    trait Irrelevant {}

    impl MyTrait for Box<dyn ObjectTrait> {
        fn use_self(&self) -> &() { panic!() }
    }
    impl Irrelevant for Box<dyn ObjectTrait> {}

    fn use_it<'a>(val: &'a Box<dyn ObjectTrait + 'a>) -> &'a () {
        val.use_self() // { dg-error ".E0772." "" { target *-*-* } }
    }
}

mod bat {
    trait OtherTrait<'a> {}
    impl<'a> OtherTrait<'a> for &'a () {}

    trait ObjectTrait {}

    impl dyn ObjectTrait {
        fn use_self(&self) -> &() { panic!() }
    }

    fn use_it<'a>(val: &'a dyn ObjectTrait) -> impl OtherTrait<'a> + 'a {
        val.use_self() // { dg-error ".E0772." "" { target *-*-* } }
    }
}

mod ban {
    trait OtherTrait<'a> {}
    impl<'a> OtherTrait<'a> for &'a () {}

    trait ObjectTrait {}
    trait MyTrait {
        fn use_self(&self) -> &() { panic!() }
    }
    trait Irrelevant {
        fn use_self(&self) -> &() { panic!() }
    }

    impl MyTrait for dyn ObjectTrait {}

    fn use_it<'a>(val: &'a dyn ObjectTrait) -> impl OtherTrait<'a> {
        val.use_self() // { dg-error ".E0759." "" { target *-*-* } }
    }
}

mod bal {
    trait OtherTrait<'a> {}
    impl<'a> OtherTrait<'a> for &'a () {}

    trait ObjectTrait {}
    trait MyTrait {
        fn use_self(&self) -> &() { panic!() }
    }
    trait Irrelevant {
        fn use_self(&self) -> &() { panic!() }
    }

    impl MyTrait for dyn ObjectTrait {}
    impl Irrelevant for dyn ObjectTrait {}

    fn use_it<'a>(val: &'a dyn ObjectTrait) -> impl OtherTrait<'a> + 'a {
        MyTrait::use_self(val) // { dg-error ".E0759." "" { target *-*-* } }
    }
}

fn main() {}

