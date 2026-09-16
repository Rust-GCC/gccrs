struct P { child: Option<Box<P>> }
trait PTrait {
   fn getChildOption(&self) -> Option<Box<P>>;
}

impl PTrait for P {
   fn getChildOption(&self) -> Option<Box<P>> {
       static childVal: Box<P> = self.child.get();
// { dg-error ".E0435." "" { target *-*-* } .-1 }
       panic!();
   }
}

fn main() {}

