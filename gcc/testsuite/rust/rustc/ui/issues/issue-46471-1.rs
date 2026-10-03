fn main() {
    let y = {
        let mut z = 0;
        &mut z
    };
// { dg-error ".E0597." "" { target *-*-* } .-2 }
    println!("{}", y);
}

