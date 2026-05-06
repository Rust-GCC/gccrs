// PR c++/115852
// { dg-do compile { target c++23 } }

template<class...Ts0>
auto x(Ts0&&...  ) requires requires {
    []<class Self>(this Self&&) {

    };
} {
    return 0;
}

int main() {
    return x(1);
}
