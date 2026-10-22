fn part(_: u16) -> u32 {
    1
}

fn main() {
    for n in 100_000.. {
// { dg-error "" "" { target *-*-* } .-1 }
        let _ = part(n);
    }
}

