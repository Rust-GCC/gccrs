pub   bar<'a>(&self, _s: &'a usize) -> bool { true }
// { dg-error "" "" { target *-*-* } .-1 }

fn main() {
    bar(2);
}

