fn main() {
    let _ = match 0 {
      0 => {
        0
      } + 5 // { dg-error "" "" { target *-*-* } }
    };
}

