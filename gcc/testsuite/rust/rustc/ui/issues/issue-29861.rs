pub trait MakeRef<'a> {
    type Ref;
}
impl<'a, T: 'a> MakeRef<'a> for T {
    type Ref = &'a T;
}

pub trait MakeRef2 {
    type Ref2;
}
impl<'a, T: 'a> MakeRef2 for T {
// { dg-error ".E0207." "" { target *-*-* } .-1 }
    type Ref2 = <T as MakeRef<'a>>::Ref;
}

fn foo() -> <String as MakeRef2>::Ref2 { &String::from("foo") }

fn main() {
    println!("{}", foo());
}

