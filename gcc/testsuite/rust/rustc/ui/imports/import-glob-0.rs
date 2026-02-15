use module_of_many_things::*;

mod module_of_many_things {
    pub fn f1() { println!("f1"); }
    pub fn f2() { println!("f2"); }
    fn f3() { println!("f3"); }
    pub fn f4() { println!("f4"); }
}


fn main() {
    f1();
    f2();
    f999(); // { dg-error ".E0425." "" { target *-*-* } }
    f4();
}

