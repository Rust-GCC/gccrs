struct MyStruct;
impl MyStruct {
    fn f() {|x, y} // { dg-error "" "" { target *-*-* } }
}

fn main() {}

