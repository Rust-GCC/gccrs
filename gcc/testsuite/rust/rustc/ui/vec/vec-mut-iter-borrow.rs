fn main() {
    let mut xs: Vec<isize> = vec![];

    for x in &mut xs {
        xs.push(1) // { dg-error ".E0499." "" { target *-*-* } }
    }
}

