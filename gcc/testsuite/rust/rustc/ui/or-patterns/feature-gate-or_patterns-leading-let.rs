// Test feature gating for a sole leading `|` in `let`.

fn main() {}

#[cfg(FALSE)]
fn gated_leading_vert_in_let() {
    let | A; // { dg-error ".E0658." "" { target *-*-* } }
}

