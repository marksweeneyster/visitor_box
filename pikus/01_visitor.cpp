// Basic Visitor
#include <iostream>
#include <string>
#include <string_view>

class Cat;
class Dog;

class PetVisitor {
public:
  virtual void visit(Cat* c) = 0;
  virtual void visit(Dog* d) = 0;
};

class Pet {
public:
  virtual ~Pet() {}
  Pet(std::string_view color)
      : color_(color) {
  }// For C++14, replace std::string_view with const std::string&
  const std::string& color() const { return color_; }
  virtual void accept(PetVisitor& v) = 0;

private:
  std::string color_;
};

class Cat : public Pet {
public:
  Cat(std::string_view color) : Pet(color) {}
  void accept(PetVisitor& v) override { v.visit(this); }
};

class Dog : public Pet {
public:
  Dog(std::string_view color) : Pet(color) {}
  void accept(PetVisitor& v) override { v.visit(this); }
};

class FeedingVisitor : public PetVisitor {
public:
  void visit(Cat* c) override {
    std::cout << "Feed tuna to the " << c->color() << " cat" << std::endl;
  }
  void visit(Dog* d) override {
    std::cout << "Feed steak to the " << d->color() << " dog" << std::endl;
  }
};

class PlayingVisitor : public PetVisitor {
public:
  void visit(Cat* c) override {
    std::cout << "Play with feather with the " << c->color() << " cat"
              << std::endl;
  }
  void visit(Dog* d) override {
    std::cout << "Play fetch with the " << d->color() << " dog" << std::endl;
  }
};

int main() {
  Cat c("orange");
  Dog d("brown");

  FeedingVisitor fv;
  c.accept(fv);
  d.accept(fv);

  PlayingVisitor pv;
  c.accept(pv);
  d.accept(pv);
}
