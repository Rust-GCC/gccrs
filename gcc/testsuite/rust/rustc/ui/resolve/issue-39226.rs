struct Handle {}

struct Something {
    handle: Handle
}

fn main() {
    let handle: Handle = Handle {};

    let s: Something = Something {
        handle: Handle
// { dg-error ".E0423." "" { target *-*-* } .-1 }
    };
}

