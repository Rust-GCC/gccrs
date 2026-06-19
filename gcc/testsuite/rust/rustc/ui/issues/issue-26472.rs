mod sub {
    pub struct S { len: usize }
    impl S {
        pub fn new() -> S { S { len: 0 } }
        pub fn len(&self) -> usize { self.len }
    }
}

fn main() {
    let s = sub::S::new();
    let v = s.len; // { dg-error ".E0616." "" { target *-*-* } }
    s.len = v; // { dg-error ".E0616." "" { target *-*-* } }
}

