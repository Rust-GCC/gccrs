// aux-build:issue-57271-lib.rs

extern crate issue_57271_lib;

use issue_57271_lib::BaseType;

pub enum ObjectType { // { dg-error ".E0072." "" { target *-*-* } }
    Class(ClassTypeSignature),
    Array(TypeSignature),
    TypeVariable(()),
}

pub struct ClassTypeSignature {
    pub package: (),
    pub class: (),
    pub inner: (),
}

pub enum TypeSignature { // { dg-error ".E0072." "" { target *-*-* } }
    Base(BaseType),
    Object(ObjectType),
}

fn main() {}

