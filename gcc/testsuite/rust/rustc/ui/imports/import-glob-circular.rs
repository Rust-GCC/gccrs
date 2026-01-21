mod circ1 {
    pub use circ2::f2;
    pub fn f1() { println!("f1"); }
    pub fn common() -> usize { return 0; }
}

mod circ2 {
    pub use circ1::f1;
    pub fn f2() { println!("f2"); }
    pub fn common() -> usize { return 1; }
}

mod test {
    use circ1::*;

    fn test() { f1066(); } // { dg-error ".E0425." "" { target *-*-* } }
}

fn main() {}

