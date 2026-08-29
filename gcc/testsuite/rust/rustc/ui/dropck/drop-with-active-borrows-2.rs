fn read_lines_borrowed<'a>() -> Vec<&'a str> {
    let raw_lines: Vec<String> = vec!["foo  ".to_string(), "  bar".to_string()];
    raw_lines.iter().map(|l| l.trim()).collect()
// { dg-error ".E0515." "" { target *-*-* } .-1 }
}

fn main() {
    println!("{:?}", read_lines_borrowed());
}

