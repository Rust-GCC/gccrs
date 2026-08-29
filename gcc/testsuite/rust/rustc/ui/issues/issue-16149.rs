extern {
    static externalValue: isize;
}

fn main() {
    let boolValue = match 42 {
        externalValue => true,
// { dg-error ".E0530." "" { target *-*-* } .-1 }
        _ => false
    };
}

