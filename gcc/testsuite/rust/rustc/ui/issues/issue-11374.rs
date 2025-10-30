use std::io::{self, Read};
use std::vec;

pub struct Container<'a> {
    reader: &'a mut dyn Read
}

impl<'a> Container<'a> {
    pub fn wrap<'s>(reader: &'s mut dyn io::Read) -> Container<'s> {
        Container { reader: reader }
    }

    pub fn read_to(&mut self, vec: &mut [u8]) {
        self.reader.read(vec);
    }
}

pub fn for_stdin<'a>() -> Container<'a> {
    let mut r = io::stdin();
    Container::wrap(&mut r as &mut dyn io::Read)
}

fn main() {
    let mut c = for_stdin();
    let mut v = Vec::new();
    c.read_to(v); // { dg-error ".E0308." "" { target *-*-* } }
}

