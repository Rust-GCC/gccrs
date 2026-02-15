fn assert_static<T: 'static>(_t: T) {}

fn main() {
    let line = String::new();
    match [&*line] { // { dg-error ".E0597." "" { target *-*-* } }
        [ word ] => { assert_static(word); }
    }
}

