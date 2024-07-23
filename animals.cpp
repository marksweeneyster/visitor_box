#include <array>
#include <format>
#include <iostream>
#include <string>
#include <variant>

/**
 * Static polymorphism and std::variant so we can aggregate different types into a single container
 */

class Cow
{
public:
    const char *see() const { return "cow"; }
    const char *say() const { return "moo"; }
};

class Pig
{
public:
    const char *see() const { return "pig"; }
    const char *say() const { return "oink"; }
};

class Dog
{
public:
    const char *see() const { return "dog"; }
    const char *say() const { return "woof"; }
};

using var_animal_t = std::variant<Cow, Pig, Dog>;

void VisitSeeSay(const var_animal_t &component) {
    auto see_and_say = [](auto &&animal) {
        std::cout << std::format("The {} says '{}!'\n", animal.see(), animal.say());
    };
    if (std::holds_alternative<Cow>(component)) {
        see_and_say( std::get<Cow>(component));
    } else if (std::holds_alternative<Pig>(component)) {
        see_and_say( std::get<Pig>(component));
    } else if (std::holds_alternative<Dog>(component)) {
        see_and_say( std::get<Dog>(component));
    } else {
        std::cout << "unsupported variant in VisitSeeSay\n";
    }
}

int main() {
    Cow cow;
    Pig pig;
    Dog dog;
    std::array<var_animal_t, 3> animals = {cow, pig, dog};

    for (const auto& animal : animals) {
        std::visit(VisitSeeSay, animal);
    }

    return 0;
}