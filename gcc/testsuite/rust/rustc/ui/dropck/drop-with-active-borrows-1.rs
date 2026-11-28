fn main() {
    let a = "".to_string();
    let b: Vec<&str> = a.lines().collect();
    drop(a);    // { dg-error ".E0505." "" { target *-*-* } }
    for s in &b {
        println!("{}", *s);
    }
}

