struct NotCopyable;

fn func<F: FnMut() -> H, H: FnMut()>(_: F) {}

fn parse() {
    let mut var = None;
    func(|| {
        // Shouldn't suggest `move ||.as_ref()` here
        move || {
// { dg-error ".E0507." "" { target *-*-* } .-1 }
            var = Some(NotCopyable);
        }
    });
}

fn main() {}

