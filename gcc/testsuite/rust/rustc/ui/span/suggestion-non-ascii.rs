fn main() {
    let tup = (1,);
    println!("☃{}", tup[0]); // { dg-error ".E0608." "" { target *-*-* } }
}

