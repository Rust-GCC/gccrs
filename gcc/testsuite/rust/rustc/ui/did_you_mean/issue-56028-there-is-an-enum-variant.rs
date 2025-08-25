enum PutDown { Set }
enum AffixHeart { Set }
enum CauseToBe { Set }
enum Determine { Set }
enum TableDishesAction { Set }
enum Solidify { Set }
enum UnorderedCollection { Set }

fn setup() -> Set { Set }
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { dg-error ".E0425." "" { target *-*-* } .-2 }

fn main() {
    setup();
}

