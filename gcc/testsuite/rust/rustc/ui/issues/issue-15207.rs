fn main() {
    loop {
        break.push(1) // { dg-error ".E0599." "" { target *-*-* } }
        ;
    }
}

