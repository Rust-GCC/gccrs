use std::vec::IntoIter;

pub fn get_tok(it: &mut IntoIter<u8>) {
    let mut found_e = false;

    let temp: Vec<u8> = it.take_while(|&x| {
        found_e = true;
        false
    })
        .cloned()
// { dg-error ".E0271." "" { target *-*-* } .-1 }
// { dg-error ".E0271." "" { target *-*-* } .-2 }
// { dg-error ".E0271." "" { target *-*-* } .-3 }
        .collect(); // { dg-error ".E0599." "" { target *-*-* } }
}

fn main() {}

