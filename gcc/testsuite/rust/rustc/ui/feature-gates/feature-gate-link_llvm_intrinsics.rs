extern {
    #[link_name = "llvm.sqrt.f32"]
    fn sqrt(x: f32) -> f32;
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

fn main(){
}

