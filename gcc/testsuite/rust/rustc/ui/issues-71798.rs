fn test_ref(x: &u32) -> impl std::future::Future<Output = u32> + '_ {
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() {
    let _ = test_ref & u; // { dg-error ".E0425." "" { target *-*-* } }
}

