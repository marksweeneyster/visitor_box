// 13 with tuple used for typelist
#include <iostream>
#include <memory>
#include <string_view>
#include <tuple>

// ---------------------------------
// Generic Visitor base
template<typename... Types>
class Visitor;

template<typename T>
class Visitor<T> {
public:
  virtual void visit(T* t) = 0;
};

template<typename T, typename... Types>
class Visitor<T, Types...> : public Visitor<Types...> {
public:
  using Visitor<Types...>::visit;
  virtual void visit(T* t) = 0;
};
//
// ---------------------------------

using PetVisitor = Visitor<class Cat, class Dog>;

// ----------------------------------------------
//
template<typename Base, typename...>
class LambdaVisitor;

template<typename Base, typename F1>
class LambdaVisitor<Base, std::tuple<F1>> : public Base, public F1 {
public:
  using F1::operator();
  explicit LambdaVisitor(F1&& f1) : F1(std::move(f1)) {}
  explicit LambdaVisitor(const F1& f1) : F1(f1) {}
};

template<typename Base, typename F1, typename... F>
class LambdaVisitor<Base, std::tuple<F1, F...>>
    : public F1, public LambdaVisitor<Base, std::tuple<F...>> {
public:
  using F1::operator();
  using LambdaVisitor<Base, std::tuple<F...>>::operator();
  explicit LambdaVisitor(F1&& f1, F&&... f)
      : F1(std::move(f1)),
        LambdaVisitor<Base, std::tuple<F...>>(std::forward<F>(f)...) {}
  explicit LambdaVisitor(const F1& f1, F&&... f)
      : F1(f1), LambdaVisitor<Base, std::tuple<F...>>(std::forward<F>(f)...) {}
};

template<typename Base, typename T1, typename... F>
class LambdaVisitor<Base, Visitor<T1>, std::tuple<F...>>
    : public LambdaVisitor<Base, std::tuple<F...>> {
public:
  using LambdaVisitor<Base, std::tuple<F...>>::LambdaVisitor;
  using LambdaVisitor<Base, std::tuple<F...>>::visit;
  void visit(T1* t) override { return this->operator()(t); }
};

template<typename Base, typename T1, typename... T, typename... F>
class LambdaVisitor<Base, Visitor<T1, T...>, std::tuple<F...>>
    : public LambdaVisitor<Base, Visitor<T...>, std::tuple<F...>> {
public:
  using LambdaVisitor<Base, Visitor<T...>, std::tuple<F...>>::LambdaVisitor;
  using LambdaVisitor<Base, Visitor<T...>, std::tuple<F...>>::visit;
  void visit(T1* t) override { return this->operator()(t); }
};

template<typename Base, typename... F>
auto lambda_visitor(F&&... f) {
  return LambdaVisitor<Base, Base, std::tuple<F...>>(std::forward<F>(f)...);
}
//
// ----------------------------------------------

class Pet {
public:
  virtual ~Pet() = default;
  explicit Pet(std::string_view color) : color_(color) {}
  const std::string& color() const { return color_; }
  virtual void accept(PetVisitor& v) = 0;

private:
  std::string color_;
};

template<typename Derived>
class Visitable : public Pet {
public:
  using Pet::Pet;
  void accept(PetVisitor& v) override { v.visit(static_cast<Derived*>(this)); }
};

class Cat : public Visitable<Cat> {
  using Visitable<Cat>::Visitable;
};

class Dog : public Visitable<Dog> {
  using Visitable<Dog>::Visitable;
};

class FeedingVisitor : public PetVisitor {
public:
  void visit(Cat* c) override {
    std::cout << "Feed tuna to the " << c->color() << " cat\n";
  }
  void visit(Dog* d) override {
    std::cout << "Feed steak to the " << d->color() << " dog\n";
  }
};

class PlayingVisitor : public PetVisitor {
public:
  void visit(Cat* c) override {
    std::cout << "Play with feather with the " << c->color() << " cat\n";
  }
  void visit(Dog* d) override {
    std::cout << "Play fetch with the " << d->color() << " dog\n";
  }
};

void walk(Pet& p) {
  auto v(lambda_visitor<PetVisitor>(
          [](Cat* c) {
            std::cout << "Let the " << c->color() << " cat out\n";
          },
          [](Dog* d) {
            std::cout << "Take the " << d->color() << " dog for a walk\n";
          }));
  p.accept(v);
}

int main() {
  std::unique_ptr<Pet> c(new Cat("orange"));
  std::unique_ptr<Pet> d(new Dog("brown"));

  FeedingVisitor fv;
  c->accept(fv);
  d->accept(fv);

  PlayingVisitor pv;
  c->accept(pv);
  d->accept(pv);

  walk(*c);
  walk(*d);
}
