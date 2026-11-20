mod mod_file_disambig_aux; // { dg-error ".E0761." "" { target *-*-* } }

fn main() {
    assert_eq!(mod_file_aux::bar(), 10);
// { dg-error ".E0433." "" { target *-*-* } .-1 }
}

