use std::process::{Command, Stdio};

fn main() {
    let process = Command::new("wc")
        .stdout(Stdio::piped())
        .spawn()
        .or_else(|err| { // { dg-error ".E0282." "" { target *-*-* } }
            panic!("oh no: {:?}", err);
        }).unwrap();
}

