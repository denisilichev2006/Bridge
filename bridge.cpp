#include <iostream>
#include <string>


class Renderer {
 public:
  virtual ~Renderer() {}
  virtual std::string RenderCircle(float x, float y, float r) const = 0;
  virtual std::string RenderRect(float x, float y, float w, float h) const = 0;
};

class VectorRenderer : public Renderer {
 public:
  std::string RenderCircle(float x, float y, float r) const override {
    return "Vector: рисуем окружность (" + std::to_string((int)x) + "," +
           std::to_string((int)y) + ") r=" + std::to_string((int)r);
  }
  std::string RenderRect(float x, float y, float w, float h) const override {
    return "Vector: рисуем прямоугольник (" + std::to_string((int)x) + "," +
           std::to_string((int)y) + ") " + std::to_string((int)w) + "x" +
           std::to_string((int)h);
  }
};

class RasterRenderer : public Renderer {
 public:
  std::string RenderCircle(float x, float y, float r) const override {
    return "Raster: растеризуем окружность (" + std::to_string((int)x) + "," +
           std::to_string((int)y) + ") r=" + std::to_string((int)r);
  }
  std::string RenderRect(float x, float y, float w, float h) const override {
    return "Raster: растеризуем прямоугольник (" + std::to_string((int)x) + "," +
           std::to_string((int)y) + ") " + std::to_string((int)w) + "x" +
           std::to_string((int)h);
  }
};

class Shape {
 protected:
  Renderer* renderer_;

 public:
  explicit Shape(Renderer* renderer) : renderer_(renderer) {}
  virtual ~Shape() {}
  virtual std::string Draw() const = 0;
  virtual std::string Resize(float factor) = 0;
};

class Circle : public Shape {
  float x_, y_, r_;

 public:
  Circle(Renderer* renderer, float x, float y, float r)
      : Shape(renderer), x_(x), y_(y), r_(r) {}

  std::string Draw() const override {
    return renderer_->RenderCircle(x_, y_, r_);
  }
  std::string Resize(float factor) override {
    r_ *= factor;
    return "Circle resized, r=" + std::to_string((int)r_);
  }
};

class Rectangle : public Shape {
  float x_, y_, w_, h_;

 public:
  Rectangle(Renderer* renderer, float x, float y, float w, float h)
      : Shape(renderer), x_(x), y_(y), w_(w), h_(h) {}

  std::string Draw() const override {
    return renderer_->RenderRect(x_, y_, w_, h_);
  }
  std::string Resize(float factor) override {
    w_ *= factor; h_ *= factor;
    return "Rectangle resized, " + std::to_string((int)w_) + "x" +
           std::to_string((int)h_);
  }
};

void ClientCode(Shape& shape) {
  std::cout << shape.Draw() << "\n";
  std::cout << shape.Resize(2.0f) << "\n";
  std::cout << shape.Draw() << "\n";
}

int main() {
  std::cout << "=== Круг + векторный рендер ===\n";
  Renderer* vr = new VectorRenderer();
  Shape* circle = new Circle(vr, 10, 20, 5);
  ClientCode(*circle);

  std::cout << "\n=== Прямоугольник + растровый рендер ===\n";
  Renderer* rr = new RasterRenderer();
  Shape* rect = new Rectangle(rr, 0, 0, 40, 30);
  ClientCode(*rect);

  std::cout << "\n=== Круг + растровый рендер (меняем реализацию) ===\n";
  Shape* circle2 = new Circle(rr, 5, 5, 10);
  std::cout << circle2->Draw() << "\n";

  delete circle; delete circle2; delete rect;
  delete vr; delete rr;
  return 0;
}
