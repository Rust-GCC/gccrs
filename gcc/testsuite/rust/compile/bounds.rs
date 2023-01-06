// { dg-additional-options "-frust-compile-until=expansion" }
#![feature(associated_type_bounds)]

trait Foo {
    type Bar;
}

trait Copy {}

trait T0 {}
trait T1 {}
trait T2 {}
trait T3 {}

fn c<F: Foo<Bar: Foo>>() where F::Bar: Copy { 
}


fn d<F: Foo<Bar: Foo 
+ T0

+ T1
+ T2 +

T3      >>() where F::Bar: Copy { 
}
