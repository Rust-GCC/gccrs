struct Parameterized1<'a> {
    g: Box<dyn FnMut() + 'a>
}

struct NotParameterized1 {
    g: Box<dyn FnMut() + 'static>
}

struct NotParameterized2 {
    g: Box<dyn FnMut() + 'static>
}

fn take1<'a>(p: Parameterized1) -> Parameterized1<'a> { p }
// { dg-error ".E0621." "" { target *-*-* } .-1 }

fn take3(p: NotParameterized1) -> NotParameterized1 { p }
fn take4(p: NotParameterized2) -> NotParameterized2 { p }

fn main() {}

