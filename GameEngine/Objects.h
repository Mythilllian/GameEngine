#pragma once

#include <windows.h>
#include <gdiplus.h>
#include <cmath>
#include <tuple>
#include <list>

using namespace std;

struct Vector {
public:
    float x;
    float y;
    Vector(float _x, float _y) : x(_x), y(_y) {}
};

struct Color {
public:
    //Red value, can range from 0-255
    int r;
    //Green value, can range from 0-255
    int g;
    //Blue value, can range from 0-255
    int b;
    Color(int _r, int _g, int _b) : r(_r), g(_g), b(_b) {}
};

enum Anchor {
    TOP_LEFT,
    TOP_MIDDLE,
    TOP_RIGHT,
    CENTER_LEFT,
    CENTER,
    CENTER_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_MIDDLE,
    BOTTOM_RIGHT
};

class Object; // Forward declaration

class Component {
public:
    Object& parent;

    virtual void Update(float deltaTime) {}

    Component(Object& _parent) : parent(_parent) {}

    Component() = delete;
};

class Transform : public Component {
public:
    Vector position;
    Vector size;
    Anchor anchor;

    int left;
    int right;
    int top;
    int bottom;

    float get_rotation() {
        return rotation;
    }
    void set_rotation(float _rotation) {
        rotation = fmod(_rotation, 360);
    }

    Transform(Object& parent, Vector _position, Vector _size, float _rotation, Anchor _anchor)
        : Component(parent), position(_position), size(_size), anchor(_anchor) {
        set_rotation(_rotation);
        CalculateCorners();
    }

    Transform(Object& parent, Vector _position, Vector _size, float _rotation)
        : Component(parent), position(_position), size(_size), anchor(BOTTOM_LEFT) {
        set_rotation(_rotation);
        CalculateCorners();
    }

    Transform(Object& parent)
        : Component(parent), position(Vector(0, 0)), size(Vector(1, 1)), anchor(BOTTOM_LEFT) {
        set_rotation(0);
        CalculateCorners();
    }

    tuple<int, int, int, int> CalculateCorners() {
        left = right = static_cast<int>(position.x);
        top = bottom = static_cast<int>(position.y);

        int n = anchor;

        left = (n % 3 == 0 ? left : (n + 1) % 3 == 0 ? left - size.x : left - size.x / 2);
        right = (n % 3 == 0 ? right + size.x : (n + 1) % 3 == 0 ? right : right + size.x / 2);
        top = (n <= 3 ? top : n >= 6 ? top + size.y : top + size.y / 2);
        bottom = (n <= 3 ? bottom - size.y : n >= 6 ? bottom : bottom - size.y / 2);

        return make_tuple(left, top, right, bottom);
    }

private:
    float rotation;
};

class Object {
public:
    list<Component*> components = {};  // Keep this list as a list of pointers
    Transform* transform;

    virtual void Draw(HDC& hdc) {}

    void Update(HDC& hdc, float deltaTime) {
        for (Component* comp : components) {
            comp->Update(deltaTime);
        }
        Draw(hdc);
    }

    Object() {
        transform = new Transform(*this);
        components.push_back(transform);
    }

    Object(Vector position, Vector size, float rotation, Anchor anchor) {
        transform = new Transform(*this, position, size, rotation, anchor);
        components.push_back(transform);
    }

    Object(Vector position, Vector size, float rotation = 0) {
        transform = new Transform(*this, position, size, rotation);
        components.push_back(transform);
    }

    virtual ~Object() {
        for (Component* comp : components) {
            delete comp;  // Fixed deletion of component pointers
        }
    }
};


enum Shape {
    RECTANGLE,
    ELLIPSE
};

class ShapeObject : public Object {
public:
    Shape shape;
    Color color;

    void Draw(HDC& hdc) override {
        auto corners = transform->CalculateCorners();

        HBRUSH redBrush = CreateSolidBrush(RGB(color.r, color.g, color.b));
        switch (shape) {
        case RECTANGLE:
            Rectangle(hdc, get<0>(corners), get<1>(corners), get<2>(corners), get<3>(corners));
            break;
        case ELLIPSE:
            Ellipse(hdc, get<0>(corners), get<1>(corners), get<2>(corners), get<3>(corners));
            break;
        }
    }

    ShapeObject(Shape _shape, Vector position, Vector size, float rotation = 0.0F, Anchor anchor = BOTTOM_LEFT, Color _color = Color(0,0,0))
        : Object(position, size, rotation, anchor), shape(_shape), color(_color) {   }
};
