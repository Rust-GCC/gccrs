// normalize-stderr-test: "not_a_real_file.rs:.*\(" -> "not_a_real_file.rs: $$FILE_NOT_FOUND_MSG ("

#[path = "not_a_real_file.rs"]
mod m; // { dg-error "" "" { target *-*-* } }

fn main() {
    assert_eq!(m::foo(), 10);
}

