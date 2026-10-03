#[derive(Debug)]
struct Pair<T, V> (T, V);

impl Pair<
    &str,
    isize
> {
    fn say(self: &Pair<&str, isize>) {
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
        println!("{:?}", self);
    }
}

fn main() {
    let result = &Pair("shane", 1);
    result.say();
}

