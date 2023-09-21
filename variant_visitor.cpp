#include <array>
#include <iostream>
#include <string>
#include <variant>
#include <tuple>


/**
 * Borrow the concrete classes from refactoring guru.
 */
class ConcreteComponentA {
public:
    std::string ExclusiveMethodOfConcreteComponentA() const {
        return "A";
    }
};

class ConcreteComponentB {
public:
    std::string SpecialMethodOfConcreteComponentB() const {
        return "B";
    }
};

using var_t = std::variant<ConcreteComponentA, ConcreteComponentB>;

void Visit1(const ConcreteComponentA& cca) {
    std::cout << cca.ExclusiveMethodOfConcreteComponentA()  << " + Visit1\n";
}

void Visit1(const ConcreteComponentB& ccb) {
    std::cout << ccb.SpecialMethodOfConcreteComponentB()  << " + Visit1\n";
}

void Visit2(const ConcreteComponentA& cca) {
    std::cout << cca.ExclusiveMethodOfConcreteComponentA()  << " + Visit2\n";
}

void Visit2(const ConcreteComponentB& ccb) {
    std::cout << ccb.SpecialMethodOfConcreteComponentB()  << " + Visit2\n";
}
/*
 * Moving the variant type deduction into a single visit function.
 * Six one way, half dozen the other
 */
void Visit3(const var_t& component) {
    if (std::holds_alternative<ConcreteComponentA>(component)) {
        auto value = std::get<ConcreteComponentA>(component);
        std::cout << value.ExclusiveMethodOfConcreteComponentA()  << " + Visit3\n";
    }
    else if (std::holds_alternative<ConcreteComponentB>(component)) {
        auto value = std::get<ConcreteComponentB>(component);
        std::cout << value.SpecialMethodOfConcreteComponentB()  << " + Visit3\n";
    } else {
        std::cout << "unsupported variant in Visit3\n";
    }
}

using visitor_type = decltype(&Visit3);

void Visit4(const var_t& component) {
    if (std::holds_alternative<ConcreteComponentA>(component)) {
        auto value = std::get<ConcreteComponentA>(component);
        std::cout << value.ExclusiveMethodOfConcreteComponentA()  << " + Visit4\n";
    }
    else if (std::holds_alternative<ConcreteComponentB>(component)) {
        auto value = std::get<ConcreteComponentB>(component);
        std::cout << value.SpecialMethodOfConcreteComponentB()  << " + Visit4\n";
    } else {
        std::cout << "unsupported variant in Visit4\n";
    }
}

void Visit5(const var_t& component) {
    if (std::holds_alternative<ConcreteComponentA>(component)) {
        auto value = std::get<ConcreteComponentA>(component);
        std::cout << value.ExclusiveMethodOfConcreteComponentA()  << " + Visit5\n";
    }
    else if (std::holds_alternative<ConcreteComponentB>(component)) {
        auto value = std::get<ConcreteComponentB>(component);
        std::cout << value.SpecialMethodOfConcreteComponentB()  << " + Visit5\n";
    } else {
        std::cout << "unsupported variant in Visit5\n";
    }
}

/*
 * Nice explanation of the magic behind "auto&& arg" commonly used with std::visit:
 *   https://levelup.gitconnected.com/understanding-std-visit-in-c-a-type-safe-way-to-traverse-variant-objects-dbeff9b47003
 */

int main() {
    ConcreteComponentA cca;
    ConcreteComponentB ccb;
    std::array<var_t,2> components = {cca, ccb};

    for (const auto& component : components) {
        /**
         * The auto&& syntax in the lambda function tells the compiler to deduce
         * the type of the argument and create a forwarding reference to it,
         * allowing us to call the correct overload for each variant type.
         */
        auto foo = [](auto&& arg){ Visit1(arg); };
        std::visit( foo, component);
    }

    std::cout << '\n';
    // same as above but without naming the lambda
    for (const auto& component : components) {
        std::visit([](auto&& arg){ Visit2(arg); }, component);
    }

    std::cout << '\n';
    std::array<visitor_type,3> visitors = {Visit3, Visit4, Visit5};

    for (const auto& visitor : visitors) {
        for (const auto& component : components) {
            std::visit(visitor, component);
        }

        std::cout << '\n';
    }

    return 0;
}
