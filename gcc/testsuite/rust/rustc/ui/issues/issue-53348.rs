fn main() {
    let mut v = vec!["hello", "this", "is", "a", "test"];

    let v2 = Vec::new();

    v.into_iter().map(|s|s.to_owned()).collect::<Vec<_>>();

    let mut a = String::new();
    for i in v {
        a = *i.to_string();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-note ".E0308." "" { target *-*-* } .-2 }
        v2.push(a);
    }
}

