fn main() {
    let x = "hello".chars().rev().collect(); // { dg-error ".E0282." "" { target *-*-* } }
}

