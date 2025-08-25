#![feature(box_patterns)]
#![feature(box_syntax)]

struct HTMLImageData {
    image: Option<String>
}

struct ElementData {
    kind: Box<ElementKind>
}

enum ElementKind {
    HTMLImageElement(HTMLImageData)
}

enum NodeKind {
    Element(ElementData)
}

struct NodeData {
    kind: Box<NodeKind>,
}

fn main() {
    let mut id = HTMLImageData { image: None };
    let ed = ElementData { kind: box ElementKind::HTMLImageElement(id) };
    let n = NodeData {kind : box NodeKind::Element(ed)};
    // n.b. span could be better
    match n.kind {
        box NodeKind::Element(ed) => match ed.kind { // { dg-error ".E0004." "" { target *-*-* } }
            box ElementKind::HTMLImageElement(ref d) if d.image.is_some() => { true }
        },
    };
}

