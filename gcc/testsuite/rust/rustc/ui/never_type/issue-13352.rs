// ignore-cloudabi no std::process

fn foo(_: Box<dyn FnMut()>) {}

fn main() {
    foo(loop {
        std::process::exit(0);
    });
    2_usize + (loop {});
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

