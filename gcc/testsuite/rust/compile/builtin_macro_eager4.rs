#![feature(rustc_attrs)]

#[rustc_builtin_macro]
macro_rules! cfg {
  () => {{}};
}

macro_rules! a{
  () => {"test"};
}


fn main () {
  let os = "linux";
  cfg!(); // { dg-error "cfg-pattern required" }
  cfg!(1); // { dg-error "expected identifier" }
  cfg!(windows); // shoud not error
  cfg!(target_os = "linux"); //should not error
  cfg!(a!()); // {dg-error "expected 1 cfg-pattern"}
  cfg!(a!(), windows); // {dg-error "expected 1 cfg-pattern"}

}
