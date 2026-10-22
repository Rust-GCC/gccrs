pub fn main() {
    let v: Vec<isize> = vec![0, 1, 2, 3, 4, 5];
    let s: String = "abcdef".to_string();
    v[3_usize];
    v[3];
    v[3u8];  // { dg-error ".E0277." "" { target *-*-* } }
    v[3i8];  // { dg-error ".E0277." "" { target *-*-* } }
    v[3u32]; // { dg-error ".E0277." "" { target *-*-* } }
    v[3i32]; // { dg-error ".E0277." "" { target *-*-* } }
    s.as_bytes()[3_usize];
    s.as_bytes()[3];
    s.as_bytes()[3u8];  // { dg-error ".E0277." "" { target *-*-* } }
    s.as_bytes()[3i8];  // { dg-error ".E0277." "" { target *-*-* } }
    s.as_bytes()[3u32]; // { dg-error ".E0277." "" { target *-*-* } }
    s.as_bytes()[3i32]; // { dg-error ".E0277." "" { target *-*-* } }
}

