fn main() {
    let x: isize = 3;
    let y: &mut isize = &mut x; // { dg-error ".E0596." "" { target *-*-* } }
    *y = 5;
    println!("{}", *y);
}

