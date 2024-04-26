#pragma once

#include <algorithm>
#include <cmath>
#include <iterator>
#include <ostream>
#include <set>

#define PI (3.1514926)

struct Vector2D {
  float _x;
  float _y;

  Vector2D(float x = 0, float y = 0) : _x(x), _y(y) {}

  Vector2D(const Vector2D& other) : _x(other._x), _y(other._y) {}

  Vector2D operator+(const Vector2D& other) {
    return Vector2D{_x + other._x, _y + other._y};
  }

  Vector2D operator-(const Vector2D& other) {
    return Vector2D{_x - other._x, _y - other._y};
  }

  Vector2D& operator+=(const Vector2D& other) {
    _x += other._x;
    _y += other._y;

    return *this;
  }

  Vector2D& operator-=(const Vector2D& other) {
    _x -= other._x;
    _y -= other._y;

    return *this;
  }

  bool operator==(const Vector2D& other) {
    return _x == other._x && _y == other._y;
  }

  friend std::ostream& operator<<(std::ostream& out, const Vector2D& p) {
    return out << "(" << p._x << "," << p._y << ")"
               << "\n";
  }

  // 计算距离
  static float Distance(const Vector2D& p1, const Vector2D& p2) {
    return sqrtf(pow(p1._x - p2._x, 2) + pow(p1._y - p2._y, 2));
  }

  // 进行向量的旋转
  static Vector2D RotateVector(float angle, const Vector2D& p) {
    double radian = PI * angle / 180;
    auto fsin = float(sin(radian));
    auto fcos = float(cos(radian));

    return Vector2D{p._x * fcos - p._y * fsin, p._x * fsin + p._y * fcos};
  }
};

struct Transform {
  Vector2D _position;
  float _rotation;
  Vector2D _scale;
  Transform() : _position(0, 0), _rotation(0), _scale(1, 1) {}

  Transform(const Vector2D& pos, const float rotation, const Vector2D& scale)
      : _position(pos), _rotation(rotation), _scale(scale) {}

  Transform Indentity() const {
    return Transform{Vector2D{0, 0}, 0, Vector2D{1, 1}};
  }
};

class Base {
 public:
  virtual void Update() = 0;
};

// 组件基类
class Component : public Base {
 public:
  virtual void Update() override;

  // 设置绑定对象
  void setOwner(class Object* owner) { _pOwner = owner; }

  virtual void Destruct();

 protected:
  class Object* _pOwner = nullptr;
};

// 场景组件
class SceneComponent : public Component {
 public:
  virtual void Update() override;

  // 设置所属组件
  void AttachTo(SceneComponent* par);

  // 解除所属组件
  void DetachFrom(SceneComponent* par);

  virtual void Destruct() override;

  Vector2D GetLocalPosition() const { return transform._position; }
  float GetLocalRotation() const { return transform._rotation; }
  Vector2D GetLocalScale() const { return transform._scale; }

  void SetLocalPosition(const Vector2D& pos) { transform._position = pos; }
  void SetLocalRotation(float rotation) { transform._rotation = rotation; }
  void SetLocalScale(const Vector2D& scale) { transform._scale = scale; }

  Vector2D GetWorldPosition() const;
  float GetWorldRotation() const;
  Vector2D GetWorldScale() const;

  void AddPosition(const Vector2D& pos) { transform._position += pos; }
  void AddRotation(const float& rotation) { transform._rotation += rotation; }

 private:
  void ProcessDestruct();

 private:
  Transform transform;
  std::set<Component*> _children;
  SceneComponent* _parent = nullptr;  // 处理场景属性关系并且便于增删
};

class Object : public Base {
 public:
  virtual void Update() override {
    std::for_each(std::begin(_components), end(_components),
                  [](auto child) { child->Update(); });
  }

  // 将组件注册到对象容器
  void RegisterComponent(Component* component) {
    if (component != nullptr) {
      auto it = _components.find(component);

      // 不在容器中添加进去
      if (it == std::end(_components)) {
        _components.emplace(component);
      }
    }
  }

  // 从对象容器中将组件注销
  void UnRegisterComponent(Component* component) {
    if (component != nullptr) {
      auto it = _components.find(component);

      // 在容器中,从容器中删除
      if (it != std::end(_components)) {
        _components.erase(component);
      }
    }
  }

  // 创建组件，将组件绑定到对象，并注册对象容器
  template <typename T>
  T* ConstructCompoent() {
    T* com = new T;
    if (com != nullptr && static_cast<Component*>(com)) {
      com->setOwner(this);
      RegisterComponent(com);
      return com;
    }
    return nullptr;
  }

  // 设置所属对象
  void AttachTo(Object* par);

  // 解除所属对象
  void DetachFrom(Object* par);

  Vector2D GetLocalPosition() const { return _root->GetLocalPosition(); }
  float GetLocalRotation() const { return _root->GetLocalRotation(); }
  Vector2D GetLocalScale() const { return _root->GetLocalScale(); }

  void SetLocalPosition(const Vector2D& pos) {
    if (_root != nullptr) {
      _root->SetLocalPosition(pos);
    } else {
      // todo 处理错误情况
    }
  }

  void SetLocalRotation(float rotation) {
    if (_root != nullptr) {
      _root->SetLocalRotation(rotation);

    } else {
      // todo 处理错误情况
    }
  }

  void SetLocalScale(const Vector2D& scale) {
    if (_root != nullptr) {
      _root->SetLocalScale(scale);

    } else {
      // todo 处理错误情况
    }
  }

  Vector2D GetWorldPosition() const;
  float GetWorldRotation() const;
  Vector2D GetWorldScale() const;

  void AddPosition(const Vector2D& pos) { _root->AddPosition(pos); }
  void AddRotation(const float& rotation) { _root->AddRotation(rotation); }

 protected:
  SceneComponent* _root = nullptr;  // 场景根组件，赋予场景属性

 private:
  std::set<Component*> _components;  // 存储对象的所有组件
  std::set<Component>::iterator _components_iter;

  std::set<Object*> _children;
  Object* _parent = nullptr;
};