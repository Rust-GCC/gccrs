// PR c++/48537
// { dg-do compile { target c++11 } }

struct SFoo
{
  SFoo() =delete;		// { dg-message "declared" "" { target c++23_down } }
};

union UFoo			// { dg-error "deleted" "" { target c++23_down } }
{
  SFoo foo;
};

int main()
{
  UFoo();			// { dg-error "deleted" "" { target c++23_down } }
}
