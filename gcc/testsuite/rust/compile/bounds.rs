trait Foo {
    type Bar;
}

trait Copy {}

trait T0 {}
trait T1 {}
trait T2 {}
trait T3 {}

fn c<F: Foo<Bar: Foo>>() where F::Bar: Copy { // { dg-error "associated type bounds are unstable" }
}


fn d<F: Foo<Bar: Foo // { dg-error "associated type bounds are unstable" }

+ T0

+ T1
+ T2 +

T3      >>() where F::Bar: Copy { 
}
