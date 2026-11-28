fn foo(z: &mut Vec<(&u8,&u8)>, (x, y): (&u8, &u8)) {
    z.push((x,y)); // { dg-error ".E0623." "" { target *-*-* } }
// { dg-error ".E0623." "" { target *-*-* } .-1 }
}

fn main() { }

