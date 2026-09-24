struct ListNode { // { dg-error ".E0072." "" { target *-*-* } }
    head: u8,
    tail: Option<ListNode>,
}

fn main() {
}

