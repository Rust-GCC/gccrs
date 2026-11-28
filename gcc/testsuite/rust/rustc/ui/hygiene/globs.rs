#![feature(decl_macro)]

mod foo {
    pub fn f() {}
}

mod bar {
    pub fn g() {}
}

macro m($($t:tt)*) {
    $($t)*
    use foo::*;
    f();
    g(); // { dg-error ".E0425." "" { target *-*-* } }
}

fn main() {
    m! {
        use bar::*;
        g();
        f(); // { dg-error ".E0425." "" { target *-*-* } }
    }
}

n!(f);
macro n($i:ident) {
    mod foo {
        pub fn $i() -> u32 { 0 }
        pub fn f() {}

        mod test {
            use super::*;
            fn g() {
                let _: u32 = $i();
                let _: () = f();
            }
        }

        macro n($j:ident) {
            mod test {
                use super::*;
                fn g() {
                    let _: u32 = $i();
                    let _: () = f();
                    $j();
                }
            }
        }
        macro n_with_super($j:ident) {
            mod test {
                use super::*;
                fn g() {
                    let _: u32 = $i();
                    let _: () = f();
                    super::$j();
                }
            }
        }

        n!(f); // { dg-error ".E0425." "" { target *-*-* } }
        n_with_super!(f);
        mod test2 {
            super::n! {
                f // { dg-error ".E0425." "" { target *-*-* } }
            }
            super::n_with_super! {
                f
            }
        }
    }
}

