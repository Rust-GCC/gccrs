fn test(t: &dyn Iterator<Item=&u64>) -> u64 {
     t.min().unwrap() // { dg-error "" "" { target *-*-* } }
}

fn main() {
     let array = [0u64];
     test(&mut array.iter());
}

