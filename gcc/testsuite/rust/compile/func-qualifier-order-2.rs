// { dg-additional-options "-frust-edition=2018" }
#![feature(no_core)]
#![no_core]

async unsafe async fn duplicate_qualifier() {} // { dg-error "encountered duplicate function qualifier .async." }

