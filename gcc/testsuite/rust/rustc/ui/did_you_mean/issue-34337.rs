fn get(key: &mut String) { }

fn main() {
    let mut v: Vec<String> = Vec::new();
    let ref mut key = v[0];
    get(&mut key); // { dg-error ".E0596." "" { target *-*-* } }
}

