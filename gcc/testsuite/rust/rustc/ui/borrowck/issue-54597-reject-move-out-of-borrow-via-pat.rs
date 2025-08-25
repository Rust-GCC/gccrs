#![allow(dead_code)]

#[derive(Debug)]
struct Value;
impl Value {
    fn as_array(&self) -> Option<&Vec<Value>> {
        None
    }
}

fn foo(val: Value) {
    let _reviewers_original: Vec<Value> = match val.as_array() {
        Some(array) => {
            *array // { dg-error ".E0507." "" { target *-*-* } }
        }
        None => vec![]
    };
}

fn main() { }

