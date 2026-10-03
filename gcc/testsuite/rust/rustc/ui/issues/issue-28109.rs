// Make sure that label for continue and break is spanned correctly

fn main() {
    loop {
        continue
        'b // { dg-error ".E0426." "" { target *-*-* } }
        ;
        break
        'c // { dg-error ".E0426." "" { target *-*-* } }
        ;
    }
}

