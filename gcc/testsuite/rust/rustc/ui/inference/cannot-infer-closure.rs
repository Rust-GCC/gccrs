fn main() {
    let x = |a: (), b: ()| {
        Err(a)?; // { dg-error ".E0282." "" { target *-*-* } }
        Ok(b)
    };
}

