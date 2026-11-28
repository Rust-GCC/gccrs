extern "C" {
    fn lol() { // { dg-error "" "" { target *-*-* } }
        println!("");
    }
}
fn main() {}

