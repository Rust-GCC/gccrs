struct S;
impl Foo for S {
    fn parse(s:&str) {
        for c in s.chars() {
            match c {
                '0'..='9' => collect_primary(&c), // { dg-error ".E0425." "" { target *-*-* } }
// { help ".E0425." "" { target *-*-* } .-1 }
                '+' | '-' => println!("We got a sign: {}", c),
                _ => println!("Not a number!")
            }
        }
    }
}
trait Foo {
    fn collect_primary(ch:&char) { }
    fn parse(s:&str);
}
trait Bar {
    fn collect_primary(ch:&char) { }
    fn parse(s:&str) {
        for c in s.chars() {
            match c {
                '0'..='9' => collect_primary(&c), // { dg-error ".E0425." "" { target *-*-* } }
// { help ".E0425." "" { target *-*-* } .-1 }
                '+' | '-' => println!("We got a sign: {}", c),
                _ => println!("Not a number!")
            }
        }
    }
}

fn main() {}

