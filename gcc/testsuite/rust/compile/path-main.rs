#[path = "path-sub.rs"]
mod foo;

pub fn main() -> i32 {
    foo::bar()
}
