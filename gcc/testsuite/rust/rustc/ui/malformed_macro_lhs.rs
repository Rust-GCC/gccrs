macro_rules! my_precioooous {
    t => (1); // { dg-error "" "" { target *-*-* } }
}

fn main() {
    my_precioooous!();
}

