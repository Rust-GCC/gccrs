type Value = String;

fn main() {
    let f = |&Value::String(_)| (); // { dg-error ".E0599." "" { target *-*-* } }

    let vec: Vec<Value> = Vec::new();
    vec.last().map(f);
}

