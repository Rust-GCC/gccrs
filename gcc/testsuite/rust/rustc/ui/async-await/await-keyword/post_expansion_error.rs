// edition:2018

macro_rules! r#await {
    () => { println!("Hello, world!") }
}

fn main() {
    await!()
// { dg-error "" "" { target *-*-* } .-1 }
}

