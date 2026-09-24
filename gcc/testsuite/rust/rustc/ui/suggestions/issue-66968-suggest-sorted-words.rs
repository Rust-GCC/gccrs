fn main() {
    let a_longer_variable_name = 1;
    println!("{}", a_variable_longer_name); // { dg-error ".E0425." "" { target *-*-* } }
}

