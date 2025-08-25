#![feature(unsized_locals, unsized_fn_params)]
// { dg-warning "" "" { target *-*-* } .-1 }

pub trait Foo {
    fn foo(self) -> String;
}

impl Foo for str {
    fn foo(self) -> String {
        self.to_owned()
    }
}

fn drop_unsized<T: ?Sized>(_: T) {}

fn main() {
    {
        let x = "hello".to_owned().into_boxed_str();
        let y = *x;
        drop_unsized(y);
        println!("{}", &x);
// { dg-error ".E0382." "" { target *-*-* } .-1 }
        println!("{}", &y);
// { dg-error ".E0382." "" { target *-*-* } .-1 }
    }

    {
        let x = "hello".to_owned().into_boxed_str();
        let y = *x;
        y.foo();
        println!("{}", &x);
// { dg-error ".E0382." "" { target *-*-* } .-1 }
        println!("{}", &y);
// { dg-error ".E0382." "" { target *-*-* } .-1 }
    }

    {
        let x = "hello".to_owned().into_boxed_str();
        x.foo();
        println!("{}", &x);
// { dg-error ".E0382." "" { target *-*-* } .-1 }
    }
}

