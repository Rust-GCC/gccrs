use std::path::{Path, PathBuf};


fn main() {
    let _tis_an_instants_play: String = "'Tis a fond Ambush—"; // { dg-error ".E0308." "" { target *-*-* } }
    let _just_to_make_bliss: PathBuf = Path::new("/ern/her/own/surprise");
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    let _but_should_the_play: String = 2; // Perhaps surprisingly, we suggest .to_string() here
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    let _prove_piercing_earnest: Vec<usize> = &[1, 2, 3]; // { dg-error ".E0308." "" { target *-*-* } }
}

