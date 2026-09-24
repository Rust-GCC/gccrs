use std::io;

fn main(){
    let x: io::Result<()> = Ok(());
    match x {
        Err(ref e) if e.kind == io::EndOfFile {
// { dg-note "" "" { target *-*-* } .-1 }
            return
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
        }
// { dg-note "" "" { target *-*-* } .-1 }
        _ => {}
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-note "" "" { target *-*-* } .-2 }
    }
}

