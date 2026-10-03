macro_rules! failed {
    () => {{
        let x = 5 ""; // { dg-error "" "" { target *-*-* } }
    }}
}

fn main() {
    failed!();
}

