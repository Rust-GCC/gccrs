#![allow(dead_code)]

fn missing_discourses() -> Result<isize, ()> {
    Ok(1)
}

fn forbidden_narratives() -> Result<isize, ()> {
    missing_discourses()?
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

fn main() {}

