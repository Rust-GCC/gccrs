struct Cat {
  meows : usize,
}

impl Cat {
    fn sleep(&self) { loop{} }
    fn meow(&self) {
      println!("Meow");
      meows += 1; // { dg-error ".E0425." "" { target *-*-* } }
      sleep();     // { dg-error ".E0425." "" { target *-*-* } }
    }

}


 fn main() { }

