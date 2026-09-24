// Type arguments in unresolved entities (reporting errors before type checking)
// should have their types recorded.

trait Tr<T> {}

fn local_type() {
    let _: Nonexistent<u8, Assoc = u16>; // { dg-error ".E0412." "" { target *-*-* } }
}

fn ufcs_trait() {
    <u8 as Tr<u8>>::nonexistent(); // { dg-error ".E0576." "" { target *-*-* } }
}

fn ufcs_item() {
    NonExistent::Assoc::<u8>; // { dg-error ".E0433." "" { target *-*-* } }
}

fn method() {
    nonexistent.nonexistent::<u8>(); // { dg-error ".E0425." "" { target *-*-* } }
}

fn closure() {
    let _ = |a, b: _| -> _ { 0 }; // { dg-error ".E0282." "" { target *-*-* } }
}

fn main() {}

