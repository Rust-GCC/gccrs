struct Table {
    rows: [[String]],
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn f(table: &Table) -> &[String] {
    &table.rows[0]
}

fn main() {}

