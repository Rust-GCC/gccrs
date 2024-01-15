fn main() {
    let mut x = 11;
    match x {
        2..=1 => x = 6,
        // { dg-error "lower range bound must be less than or equal to upper" "" { target *-*-* } .-1 }
        _ => x = 7,
    }

    let mut y = 'a';
    match y {
        'z'..='y' => y = 'b',
        // { dg-error "lower range bound must be less than or equal to upper" "" { target *-*-* } .-1 }
        _ => y = 'c',
    }
}
