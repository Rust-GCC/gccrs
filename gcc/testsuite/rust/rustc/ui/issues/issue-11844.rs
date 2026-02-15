#![feature(box_syntax)]

fn main() {
    let a = Some(box 1);
    match a {
        Ok(a) => // { dg-error ".E0308." "" { target *-*-* } }
            println!("{}",a),
        None => panic!()
    }
}

