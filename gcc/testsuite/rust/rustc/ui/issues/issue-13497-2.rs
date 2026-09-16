fn read_lines_borrowed<'a>() -> Vec<&'a str> {
    let rawLines: Vec<String> = vec!["foo  ".to_string(), "  bar".to_string()];
    rawLines // { dg-error ".E0515." "" { target *-*-* } }
        .iter().map(|l| l.trim()).collect()
}

fn main() {}

