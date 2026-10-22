// Regression test for #27042. Test that a loop's label is included in its span.

fn main() {
    let _: i32 =
        'a: // in this case, the citation is just the `break`:
        loop { break }; // { dg-error ".E0308." "" { target *-*-* } }
    let _: i32 =
        'b: // { dg-error ".E0308." "" { target *-*-* } }
// { dg-warning ".E0308." "" { target *-*-* } .-1 }
        while true { break }; // but here we cite the whole loop
    let _: i32 =
        'c: // { dg-error ".E0308." "" { target *-*-* } }
        for _ in None { break }; // but here we cite the whole loop
    let _: i32 =
        'd: // { dg-error ".E0308." "" { target *-*-* } }
        while let Some(_) = None { break };
}

