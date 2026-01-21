fn closure_to_loc() {
    let mut x = |c| c + 1;
    x = |c| c + 1;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn closure_from_match() {
    let x = match 1usize {
        1 => |c| c + 1,
        2 => |c| c - 1,
        _ => |c| c - 1
    };
// { dg-error ".E0282." "" { target *-*-* } .-4 }
}

fn main() { }

