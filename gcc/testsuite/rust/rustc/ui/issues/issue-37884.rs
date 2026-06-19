struct RepeatMut<'a, T>(T, &'a ());

impl<'a, T: 'a> Iterator for RepeatMut<'a, T> {

    type Item = &'a mut T;
    fn next(&'a mut self) -> Option<Self::Item>
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    {
        Some(&mut self.0)
    }
}

fn main() {}

