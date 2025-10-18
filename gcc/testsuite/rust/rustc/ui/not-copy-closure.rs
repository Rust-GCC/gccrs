// Check that closures do not implement `Copy` if their environment is not `Copy`.

fn main() {
    let mut a = 5;
    let hello = || {
        a += 1;
    };

    let b = hello;
    let c = hello; // { dg-error ".E0382." "" { target *-*-* } }
}

