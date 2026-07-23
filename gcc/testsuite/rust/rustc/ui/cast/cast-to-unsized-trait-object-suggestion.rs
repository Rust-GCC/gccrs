fn main() {
    &1 as dyn Send; // { dg-error ".E0620." "" { target *-*-* } }
    Box::new(1) as dyn Send; // { dg-error ".E0620." "" { target *-*-* } }
}

