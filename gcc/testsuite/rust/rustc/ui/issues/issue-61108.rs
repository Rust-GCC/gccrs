fn main() {
    let mut bad_letters = vec!['e', 't', 'o', 'i'];
    for l in bad_letters {
        // something here
    }
    bad_letters.push('s'); // { dg-error ".E0382." "" { target *-*-* } }
}

