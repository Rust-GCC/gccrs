// Check that associated paths starting with `<<` are successfully parsed.

fn main() {
    let _: <<A>::B>::C; // { dg-error ".E0412." "" { target *-*-* } }
    let _ = <<A>::B>::C; // { dg-error ".E0412." "" { target *-*-* } }
    let <<A>::B>::C; // { dg-error ".E0412." "" { target *-*-* } }
    let 0 ..= <<A>::B>::C; // { dg-error ".E0412." "" { target *-*-* } }
    <<A>::B>::C; // { dg-error ".E0412." "" { target *-*-* } }
}

