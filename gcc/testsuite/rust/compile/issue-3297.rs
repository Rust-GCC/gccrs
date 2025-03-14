pub fn main() {
    let mut x = 42;
    x + 1= 2; // { dg-error "invalid left-hand side of assignment" }
}