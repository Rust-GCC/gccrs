fn main() {
    let _ = [(); {
        let mut x = &0;
        let mut n = 0;
        while n < 5 {
            n = (n + 1) % 5; // { dg-error ".E0080." "" { target *-*-* } }
            x = &0; // Materialize a new AllocId
        }
        0
    }];
}

