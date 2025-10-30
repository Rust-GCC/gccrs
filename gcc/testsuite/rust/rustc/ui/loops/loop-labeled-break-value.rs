fn main() {
    loop {
        let _: i32 = loop { break }; // { dg-error ".E0308." "" { target *-*-* } }
    }
    loop {
        let _: i32 = 'inner: loop { break 'inner }; // { dg-error ".E0308." "" { target *-*-* } }
    }
    loop {
        let _: i32 = 'inner2: loop { loop { break 'inner2 } }; // { dg-error ".E0308." "" { target *-*-* } }
    }
}

