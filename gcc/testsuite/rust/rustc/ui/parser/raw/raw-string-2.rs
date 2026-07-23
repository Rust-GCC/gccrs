fn main() {
    let x = r###"here's a long string"# "# "##;
// { dg-error ".E0748." "" { target *-*-* } .-1 }
}

