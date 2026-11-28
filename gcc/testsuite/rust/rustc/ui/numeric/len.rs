fn main() {
    let array = [1, 2, 3];
    test(array.len()); // { dg-error ".E0308." "" { target *-*-* } }
}

fn test(length: u32) {
    println!("{}", length);
}

