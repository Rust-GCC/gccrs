// check-pass

// pretty-expanded FIXME #23616

pub use local as local_alias;

pub mod local { }

pub fn main() {}

