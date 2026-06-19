// #66353: ICE when trying to recover from incorrect associated type

trait _Func<T> {
    fn func(_: Self);
}

trait _A {
    type AssocT;
}

fn main() {
    _Func::< <() as _A>::AssocT >::func(());
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
}

