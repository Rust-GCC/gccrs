struct Point<T> {
    x: T,
    y: T,
}

type PointF = Point<f32>;

struct Pair<T,U> {
    x: T,
    y: U,
}

type PairF<U> = Pair<f32,U>;

fn main() {
    let pt = PointF {
        x: 1,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
        y: 2,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    };

    let pt2 = Point::<f32> {
        x: 3,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
        y: 4,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
    };

    let pair = PairF {
        x: 5,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
        y: 6,
    };

    let pair2 = PairF::<i32> {
        x: 7,
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
        y: 8,
    };

    let pt3 = PointF::<i32> { // { dg-error ".E0107." "" { target *-*-* } }
        x: 9,  // { dg-error ".E0308." "" { target *-*-* } }
        y: 10, // { dg-error ".E0308." "" { target *-*-* } }
    };

    match (Point { x: 1, y: 2 }) {
        PointF::<u32> { .. } => {} // { dg-error ".E0308." "" { target *-*-* } }
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    }

    match (Point { x: 1, y: 2 }) {
        PointF { .. } => {} // { dg-error ".E0308." "" { target *-*-* } }
    }

    match (Point { x: 1.0, y: 2.0 }) {
        PointF { .. } => {} // ok
    }

    match (Pair { x: 1, y: 2 }) {
        PairF::<u32> { .. } => {} // { dg-error ".E0308." "" { target *-*-* } }
    }

    match (Pair { x: 1.0, y: 2 }) {
        PairF::<u32> { .. } => {} // ok
    }
}

