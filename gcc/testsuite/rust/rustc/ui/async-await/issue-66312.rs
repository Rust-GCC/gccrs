// edition:2018

trait Test<T> {
    fn is_some(self: T); // { dg-error ".E0307." "" { target *-*-* } }
}

async fn f() {
    let x = Some(2);
    if x.is_some() {
        println!("Some");
    }
}

fn main() {}

