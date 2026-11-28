#![feature(box_patterns)]
#![feature(box_syntax)]

trait MyTrait {
    fn dummy(&self) {}
}

pub enum TraitWrapper {
    A(Box<dyn MyTrait + 'static>),
}

fn get_tw_map(tw: &TraitWrapper) -> &dyn MyTrait {
    match *tw {
        TraitWrapper::A(box ref map) => map, // { dg-error ".E0033." "" { target *-*-* } }
    }
}

pub fn main() {}

