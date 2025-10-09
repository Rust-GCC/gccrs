trait Wedding<'t>: 't { }

struct Prince<'kiss, 'SnowWhite> {
    child: Box<dyn Wedding<'kiss> + 'SnowWhite>, // { dg-error ".E0478." "" { target *-*-* } }
}

fn main() {
}

