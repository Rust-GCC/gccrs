fn changer<'a>(mut things: Box<dyn Iterator<Item=&'a mut u8>>) {
    for item in *things { *item = 0 }
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {}

