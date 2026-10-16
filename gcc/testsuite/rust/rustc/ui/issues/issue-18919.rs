type FuncType<'f> = dyn Fn(&isize) -> isize + 'f;

fn ho_func(f: Option<FuncType>) {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

enum Option<T> {
    Some(T),
    None,
}

fn main() {}

