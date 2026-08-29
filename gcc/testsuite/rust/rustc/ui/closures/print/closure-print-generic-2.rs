mod mod1 {
    pub fn f<T: std::fmt::Display>(t: T) {
        let x = 20;

        let c = || println!("{} {}", t, x);
        let c1: () = c;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    }
}

fn main() {
    mod1::f(5i32);
}

