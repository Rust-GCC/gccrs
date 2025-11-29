// The type of `y` ends up getting inferred to the type of the block.
fn broken() {
    let mut x = 3;
    let mut _y = vec![&mut x];
    while x < 10 { // { dg-error ".E0503." "" { target *-*-* } }
        let mut z = x; // { dg-error ".E0503." "" { target *-*-* } }
        _y.push(&mut z);
// { dg-error ".E0597." "" { target *-*-* } .-1 }
        x += 1; // { dg-error ".E0503." "" { target *-*-* } }
    }
}

fn main() { }

