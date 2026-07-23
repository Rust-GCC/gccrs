// It should just use the entire body instead of pointing at the next two lines
struct // { dg-error ".E0072." "" { target *-*-* } }
ListNode
{
    head: u8,
    tail: Option<ListNode>,
}

fn main() {
}

