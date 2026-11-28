fn main() {
    let a = vec![1, 2, 3];
    for i in &a {
        for j in a {
// { dg-error ".E0382." "" { target *-*-* } .-1 }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
            println!("{} * {} = {}", i, j, i * j);
        }
    }
}

