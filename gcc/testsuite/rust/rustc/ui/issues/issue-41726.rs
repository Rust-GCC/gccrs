use std::collections::HashMap;
fn main() {
    let things: HashMap<String, Vec<String>> = HashMap::new();
    for src in things.keys() {
        things[src.as_str()].sort(); // { dg-error ".E0596." "" { target *-*-* } }
    }
}

