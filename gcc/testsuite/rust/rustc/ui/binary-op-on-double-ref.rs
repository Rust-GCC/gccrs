// run-rustfix
fn main() {
    let v = vec![1, 2, 3, 4, 5, 6, 7, 8, 9];
    let vr = v.iter().filter(|x| {
        x % 2 == 0
// { dg-error ".E0369." "" { target *-*-* } .-1 }
    });
    println!("{:?}", vr);
}

