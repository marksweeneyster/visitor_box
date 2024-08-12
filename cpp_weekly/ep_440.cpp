// https://www.youtube.com/watch?v=et1fjd8X1ho

#include <iostream>
#include <string_view>
#include <variant>

namespace {
  using var_t = std::variant<int, float, std::string_view>;

  template<typename... Callable>
  struct Visitor : Callable... {
    using Callable::operator()...;
  };
}


int main() {
  var_t arr[] = {"hola", 41.999f, 42};

  // instantiate a Visitor
  Visitor visitor{
          [](int i) { std::cout << "Int " << i << '\n'; },
          [](float f) { std::cout << "Float " << f << '\n'; },
          [](std::string_view sv) { std::cout << "SV " << sv << '\n'; }};

  for (const auto& val: arr) {
    std::visit(visitor, val);
  }
}

/**
 * https://cppinsights.io/s/c9b46e4e
 *
    template<>
    struct Visitor<__lambda_21_13, __lambda_22_13, __lambda_23_13> :
        public __lambda_21_13, public __lambda_22_13, public __lambda_23_13
    {
      using __lambda_21_13::operator();

      using __lambda_22_13::operator();

      using __lambda_23_13::operator();
    };
 *
 *
*/