struct Foo {}

impl Foo {
    fn foo(&self) {
        bar(self);
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { help ".E0425." "" { target *-*-* } .-2 }

        bar(&&self, 102);
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { help ".E0425." "" { target *-*-* } .-2 }

        bar(&mut self, 102, &"str");
// { dg-error ".E0425." "" { target *-*-* } .-1 }
// { help ".E0425." "" { target *-*-* } .-2 }

        bar();
// { dg-error ".E0425." "" { target *-*-* } .-1 }

        self.bar();
// { dg-error ".E0599." "" { target *-*-* } .-1 }
    }
}

fn main() {}

