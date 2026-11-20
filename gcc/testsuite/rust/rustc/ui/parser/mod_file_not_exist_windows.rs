// only-windows

mod not_a_real_file; // { dg-error ".E0583." "" { target *-*-* } }
// { help ".E0583." "" { target *-*-* } .-1 }

fn main() {
    assert_eq!(mod_file_aux::bar(), 10);
// { dg-error ".E0433." "" { target *-*-* } .-1 }
}

