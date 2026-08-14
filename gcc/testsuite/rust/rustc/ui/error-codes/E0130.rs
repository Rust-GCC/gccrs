extern {
    fn foo((a, b): (u32, u32));
// { dg-error ".E0130." "" { target *-*-* } .-1 }
}

fn main() {
}

