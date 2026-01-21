fn main() {
    let x = (vec![1, 2, 3], );
    drop(x.0);
    drop(x); // { dg-error ".E0382." "" { target *-*-* } }
}

