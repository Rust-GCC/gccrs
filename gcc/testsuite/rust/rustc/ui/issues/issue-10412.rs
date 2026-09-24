trait Serializable<'self, T> { // { dg-error "" "" { target *-*-* } }
    fn serialize(val : &'self T) -> Vec<u8>; // { dg-error "" "" { target *-*-* } }
    fn deserialize(repr : &[u8]) -> &'self T; // { dg-error "" "" { target *-*-* } }
}

impl<'self> Serializable<str> for &'self str { // { dg-error ".E0277." "" { target *-*-* } }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
// { dg-error ".E0277." "" { target *-*-* } .-3 }
    fn serialize(val : &'self str) -> Vec<u8> { // { dg-error "" "" { target *-*-* } }
        vec![1]
    }
    fn deserialize(repr: &[u8]) -> &'self str { // { dg-error "" "" { target *-*-* } }
        "hi"
    }
}

fn main() {
    println!("hello");
    let x = "foo".to_string();
    let y = x;
    println!("{}", y);
}

