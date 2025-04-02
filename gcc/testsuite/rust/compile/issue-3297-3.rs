pub fn main() {
    let mut x = 42;
    x + 1 += 1; // { dg-error "invalid left-hand side of assignment" }
}