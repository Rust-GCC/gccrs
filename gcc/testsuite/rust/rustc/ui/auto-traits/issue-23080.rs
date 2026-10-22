#![feature(optin_builtin_traits)]
#![feature(negative_impls)]

unsafe auto trait Trait {
    fn method(&self) { // { dg-error ".E0380." "" { target *-*-* } }
        println!("Hello");
    }
}

fn call_method<T: Trait>(x: T) {
    x.method();
}

fn main() {
    // ICE
    call_method(());
}

