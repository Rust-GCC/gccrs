enum Functions {
    Square = |x| x, // { dg-error ".E0308." "" { target *-*-* } }
}

fn main() {}

