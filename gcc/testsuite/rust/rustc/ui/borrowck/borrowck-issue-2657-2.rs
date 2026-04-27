#![feature(box_syntax)]

fn main() {
    let x: Option<Box<_>> = Some(box 1);
    match x {
      Some(ref y) => {
        let _b = *y; // { dg-error ".E0507." "" { target *-*-* } }
      }
      _ => {}
    }
}

