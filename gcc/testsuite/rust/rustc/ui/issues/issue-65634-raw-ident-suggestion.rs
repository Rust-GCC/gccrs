#![allow(non_camel_case_types)]

trait r#async {
    fn r#struct(&self) {
        println!("async");
    }
}

trait r#await {
    fn r#struct(&self) {
        println!("await");
    }
}

struct r#fn {}

impl r#async for r#fn {}
impl r#await for r#fn {}

fn main() {
    r#fn {}.r#struct(); // { dg-error ".E0034." "" { target *-*-* } }
}

