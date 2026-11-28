trait Animal {
  fn eat(&self);
}

struct Cat {
  meows: usize,
}

impl Animal for Cat {
// { dg-error ".E0046." "" { target *-*-* } .-1 }
}

fn cat(in_x : usize) -> Cat {
    Cat {
        meows: in_x
    }
}

fn main() {
  let nyan = cat(0);
}

