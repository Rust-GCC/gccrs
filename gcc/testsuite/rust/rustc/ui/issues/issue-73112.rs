// aux-build:issue-73112.rs

extern crate issue_73112;

fn main() {
    use issue_73112::PageTable;

    #[repr(C, packed)]
    struct SomeStruct {
// { dg-error ".E0588." "" { target *-*-* } .-1 }
        page_table: PageTable,
    }
}

