fn main() {
    let sixteen: f32 = 16;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
    let a_million_and_seventy: f64 = 1_000_070;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }
    let negative_nine: f32 = -9;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { help ".E0308." "" { target *-*-* } .-2 }


    // only base-10 literals get the suggestion

    let sixteen_again: f64 = 0x10;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    let and_once_more: f32 = 0o20;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

