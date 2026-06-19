fn main() {
    let a = std::process::Command::new("echo")
        .arg("1")
        ,arg("2") // { dg-error "" "" { target *-*-* } }
        .output();
}

