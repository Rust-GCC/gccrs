fn main() {
    extern {
        static symbol: [usize]; // { dg-error ".E0277." "" { target *-*-* } }
    }
    println!("{}", symbol[0]);
}

