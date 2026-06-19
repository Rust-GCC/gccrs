fn read_lines_borrowed1() -> Vec<
    &str // { dg-error ".E0106." "" { target *-*-* } }
> {
    let rawLines: Vec<String> = vec!["foo  ".to_string(), "  bar".to_string()];
    rawLines.iter().map(|l| l.trim()).collect()
}

fn main() {}

