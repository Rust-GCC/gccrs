struct NonCopyable(());

fn main() {
    let z = NonCopyable{ p: () }; // { dg-error ".E0560." "" { target *-*-* } }
}

