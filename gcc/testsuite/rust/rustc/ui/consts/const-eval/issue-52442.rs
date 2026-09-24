fn main() {
    [();  { &loop { break } as *const _ as usize } ];
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// { dg-error ".E0080." "" { target *-*-* } .-2 }
}

