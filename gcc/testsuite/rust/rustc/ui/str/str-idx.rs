pub fn main() {
    let s: &str = "hello";
    let _: u8 = s[4]; // { dg-error ".E0277." "" { target *-*-* } }
    let _ = s.get(4); // { dg-error ".E0277." "" { target *-*-* } }
    let _ = s.get_unchecked(4); // { dg-error ".E0277." "" { target *-*-* } }
    let _: u8 = s['c']; // { dg-error ".E0277." "" { target *-*-* } }
}

