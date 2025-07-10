#include <iostream>
#include <vector>
#include <memory>

// Base class with a virtual function
class Shape {
public:
    // A virtual destructor is IMPORTANT if we delete objects via a base class pointer
    virtual ~Shape() = default;
    virtual void draw() const {
        std::cout << "Drawing a generic shape." << std::endl;
    }
};

// Derived class 1
class Circle : public Shape {
public:
    void draw() const override { // `override` is a good practice
        std::cout << "Drawing a circle: O" << std::endl;
    }
};

// Derived class 2
class Square : public Shape {
public:
    void draw() const override {
        std::cout << "Drawing a square: []" << std::endl;
    }
};

// A function that works with any shape through the base interface
void render_scene(const std::vector<std::unique_ptr<Shape>>& shapes) {
    for (const auto& shape_ptr : shapes) {
        shape_ptr->draw(); // The V-Table mechanism works here!
    }
}

int main() {
    std::vector<std::unique_ptr<Shape>> scene;

    scene.push_back(std::make_unique<Circle>());
    scene.push_back(std::make_unique<Square>());
    
    scene.emplace_back(std::make_unique<Circle>());

    render_scene(scene);

    return 0;
}