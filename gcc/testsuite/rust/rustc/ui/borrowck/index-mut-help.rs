// When mutably indexing a type that implements `Index` but not `IndexMut`, a
// special 'help' message is added to the output.


fn main() {
    use std::collections::HashMap;

    let mut map = HashMap::new();
    map.insert("peter", "23".to_string());

    map["peter"].clear();           // { dg-error ".E0596." "" { target *-*-* } }
    map["peter"] = "0".to_string(); // { dg-error ".E0594." "" { target *-*-* } }
    let _ = &mut map["peter"];      // { dg-error ".E0596." "" { target *-*-* } }
}

