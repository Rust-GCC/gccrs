// PR c++/123613
// { dg-do compile { target c++26 } }
// { dg-additional-options "-freflection" }

#include <meta>
#include <charconv>
#include <string>
#include <algorithm>

template <std::meta::info ...Ms>
struct Outer {
    struct Inner;
    consteval {
        define_aggregate(^^Inner, {Ms...});
    }
};

template <std::meta::info ...Ms>
using Cls = Outer<Ms...>::Inner;

template <typename T, auto ... Vs>
constexpr auto construct_from = T{Vs...};

consteval std::meta::info parse_json(std::string_view json) {
  auto cursor = json.begin();
  auto end = json.end();

  auto is_whitespace = [](char c) {
    return c == ' ' || c == '\n' || c == '\t';
  };

  auto skip_whitespace = [&] -> void {
    while (is_whitespace(*cursor)) cursor++;
  };

  auto expect_consume = [&] (char c) -> void {
    skip_whitespace();
    if (cursor == end || *(cursor++) != c) throw "unexpected character";
  };

  auto parse_until = [&](std::vector<char> delims, std::string &out) -> void {
    skip_whitespace();
    while (cursor != end &&
           !std::ranges::any_of(delims, [&](char c) { return c == *cursor; }))
      out += *(cursor++);
  };

  auto parse_delimited = [&](char lhs, std::string &out, char rhs) -> void {
    skip_whitespace();
    expect_consume(lhs);
    parse_until({rhs}, out);
    expect_consume(rhs);
  };

  auto parse_value = [&](std::string &out) -> void {
    skip_whitespace();

    bool quoted = false;
    unsigned depth = 0;
    while (true) {
      if (cursor == end) throw "unexpected end of stream";
      if (is_whitespace(*cursor) && !quoted && depth == 0)
        break;

      if (depth == 0 && (*cursor == ',' || *cursor == '}'))
        break;
      out += *(cursor++);

      if (out.back() == '{')
        ++depth;
      else if (out.back() == '}')
        --depth;
      else if (out.back() == '"') {
        if (quoted && depth == 0)
          break;
        quoted = true;
      }
    };
  };

  skip_whitespace();
  expect_consume('{');

  std::vector<std::meta::info> members;
  std::vector<std::meta::info> values = {^^void};

  using std::meta::reflect_constant, std::meta::reflect_constant_string;
  while (cursor != end && *cursor != '}') {
    std::string field_name;
    std::string value;

    std::meta::info parsed_type;

    parse_delimited('"', field_name, '"');
    expect_consume(':');
    parse_value(value);

    if (value.empty()) throw "expected value";
    if (cursor == end) throw "unexpected end of stream";

    if (value[0] == '"') {
      if (value.back() == '}' && value[1] == 'f') throw "hmm";
      if (value.back() != '"') throw "expected end of string";
      std::string_view contents(&value[1], value.size() - 2);

      auto dms = data_member_spec(^^char const*, {.name=field_name});
      members.push_back(reflect_constant(dms));
      values.push_back(reflect_constant_string(contents));
    } else if (value[0] >= '0' && value[0] <= '9') {
      int contents = [](std::string_view in) {
        int out;
        std::from_chars(in.data(), in.data() + in.size(), out);
        return out;
      }(value);

      auto dms = data_member_spec(^^int, {.name=field_name});
      members.push_back(reflect_constant(dms));
      values.push_back(reflect_constant(contents));
    } else if (value[0] == '{') {
      std::meta::info parsed = parse_json(value);

      auto dms = data_member_spec(type_of(parsed), {.name=field_name});
      members.push_back(reflect_constant(dms));
      values.push_back(parsed);
    }
    skip_whitespace();
    if (cursor != end && *cursor == ',')
      ++cursor;
  }
  if (cursor == end) throw "hmm";
  expect_consume('}');

  values[0] = substitute(^^Cls, members);
  return substitute(^^construct_from, values);
}

struct JSONString {
    std::meta::info Rep;
    consteval JSONString(const char *Json) : Rep{parse_json(Json)} {}
};

template <JSONString json>
consteval auto operator""_json() {
    return [:json.Rep:];
}

template <JSONString json>
inline constexpr auto json_to_object = [: json.Rep :];

constexpr const char data[] = {
    #embed "test.json"
    , 0
};

int main() {
  constexpr auto v = json_to_object<data>;
  static_assert(std::string_view(v.outer) == "text");
  static_assert(v.inner.number == 2996);
  static_assert(std::string_view(v.inner.field) == "yes");
  static_assert(R"({"field": "yes", "number": 2996})"_json.number == 2996);
}
