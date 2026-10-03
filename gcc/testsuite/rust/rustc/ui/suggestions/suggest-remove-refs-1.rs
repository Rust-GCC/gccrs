// run-rustfix

fn main() {
    let v = vec![0, 1, 2, 3];

    for (i, _) in &v.iter().enumerate() {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
        println!("{}", i);
    }
}

