pub trait AbstractRenderer {}

fn _create_render(_: &()) ->
    dyn AbstractRenderer
// { dg-error ".E0746." "" { target *-*-* } .-1 }
{
    match 0 {
        _ => unimplemented!()
    }
}

fn main() {
}

