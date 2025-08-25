trait Tr {
    const C: Self;
}

fn main() {
    let a: u8 = Tr::C; // { dg-error ".E0277." "" { target *-*-* } }
}

