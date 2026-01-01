// PR c++/123346
// { dg-do compile { target c++20 } }
struct Unit {};
union Union { Unit unit; };
constexpr Union make(Union&& other) {
    return Union {.unit = other.unit };
}
constexpr Union u = make(Union { .unit = Unit{} });
