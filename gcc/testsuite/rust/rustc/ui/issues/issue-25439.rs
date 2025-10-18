struct Helper<'a, F: 'a>(&'a F);

fn fix<F>(f: F) -> i32 where F: Fn(Helper<F>, i32) -> i32 {
    f(Helper(&f), 8)
}

fn main() {
    fix(|_, x| x); // { dg-error ".E0644." "" { target *-*-* } }
}

