// Issue #8624. Test for reborrowing with 3 levels, not just two.

fn copy_borrowed_ptr<'a, 'b, 'c>(p: &'a mut &'b mut &'c mut isize) -> &'b mut isize {
    &mut ***p // { dg-error ".E0623." "" { target *-*-* } }
}

fn main() {
}

