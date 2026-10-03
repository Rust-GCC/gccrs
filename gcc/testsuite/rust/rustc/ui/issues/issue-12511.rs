trait T1 : T2 {
// { dg-error ".E0391." "" { target *-*-* } .-1 }
}

trait T2 : T1 {
}

fn main() { }

