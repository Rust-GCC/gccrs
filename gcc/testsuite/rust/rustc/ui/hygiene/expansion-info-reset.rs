fn main() {
    format_args!({ #[derive(Clone)] struct S; });
// { dg-error "" "" { target *-*-* } .-1 }
}

