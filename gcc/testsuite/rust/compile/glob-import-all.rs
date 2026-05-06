// { dg-additional-options "-frust-edition=2015" }

#![feature(no_core)]
#![no_core]

mod a {
    #[allow(unused)]
    use ::*;
}
