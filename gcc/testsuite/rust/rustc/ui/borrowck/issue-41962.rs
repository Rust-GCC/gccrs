pub fn main(){
    let maybe = Some(vec![true, true]);

    loop {
        if let Some(thing) = maybe {
        }
// { dg-error ".E0382." "" { target *-*-* } .-2 }
    }
}

