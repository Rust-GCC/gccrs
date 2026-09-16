fn bot<T>() -> T { loop {} }

fn mutate(s: &mut str) {
    s[1..2] = bot();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
// { dg-error ".E0277." "" { target *-*-* } .-2 }
    s[1usize] = bot();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    s.get_mut(1);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    s.get_unchecked_mut(1);
// { dg-error ".E0277." "" { target *-*-* } .-1 }
    s['c'];
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

pub fn main() {}

