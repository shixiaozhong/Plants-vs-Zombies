#include <algorithm>
#include <iterator>
#include <queue>

#include "include/CoreMinimal.h"

// 一个全局的World对象
extern World mainWorld;

void Component::Destruct() {
  _pOwner->UnRegisterComponent(this);
  delete this;
}

// SceneComponent

void SceneComponent::ProcessDestruct() {
  if (!_children.empty()) {
    std::for_each(std::begin(_children), std::end(_children),
                  [](auto child) { child->ProcessDestruct(); });
  }
  Component::Destruct();
}

void SceneComponent::AttachTo(SceneComponent* par) {
  if (par != nullptr) {
    par->_children.emplace(this);
    _parent = par;

    // 设置组件所属的对象
    SetOwner(par->_pOwner);
  } else {
    // todo 出现失败情况
  }
}

void SceneComponent::DetachFrom(SceneComponent* par) {
  if (par != nullptr) {
    par->_children.erase(this);
    _parent = nullptr;

    // 将组件与对象解绑
    SetOwner(nullptr);
  } else {
    // todo 出现失败情况
  }
}

void SceneComponent::Destruct() {
  // 如果有父组件的话，从父组件中删除
  if (_parent != nullptr) {
    _parent->_children.erase(this);
  }

  ProcessDestruct();
}

Vector2D SceneComponent::GetWorldPosition() const {
  if (_parent != nullptr) {
    return _parent->GetWorldPosition() + GetLocalPosition();
  } else {
    // 如果组件属于一个对象
    if (_pOwner != nullptr) {
      return _pOwner->GetWorldPosition();
    }
    return GetLocalPosition();
  }
}

float SceneComponent::GetWorldRotation() const {
  if (_parent != nullptr) {
    return _parent->GetWorldRotation() + GetLocalRotation();
  } else {
    if (_pOwner != nullptr) {
      return _pOwner->GetWorldRotation();
    }
    return GetLocalRotation();
  }
}

Vector2D SceneComponent::GetWorldScale() const {
  if (_parent != nullptr) {
    return _parent->GetWorldScale() + GetLocalScale();
  } else {
    if (_pOwner != nullptr) {
      return _pOwner->GetWorldScale();
    }
    return GetWorldScale();
  }
}

// Object

Vector2D Object::GetWorldPosition() const {
  if (_parent != nullptr) {
    return _parent->GetWorldPosition() + GetLocalPosition();
  }

  return GetLocalPosition();
}

float Object::GetWorldRotation() const {
  if (_parent != nullptr) {
    return _parent->GetWorldRotation() + GetLocalRotation();
  }
  return GetLocalRotation();
}

Vector2D Object::GetWorldScale() const {
  if (_parent != nullptr) {
    return _parent->GetWorldScale() + GetLocalScale();
  }
  return GetLocalScale();
}

void Object::AttachTo(Object* par) {
  if (par != nullptr) {
    par->_children.emplace(this);
    _parent = par;
  } else {
    // todo 出现失败情况，
  }
}

void Object::DetachFrom(Object* par) {
  if (par != nullptr) {
    par->_children.emplace(this);
    _parent = nullptr;
  } else {
    // todo 出现失败情况
  }
}

void Object::Destory() {
  if (_parent != nullptr) {
    _parent->_children.erase(this);
  }

  // bfs的方式删除所有的对象
  std::queue<Object*> objects_to_delete;
  objects_to_delete.push(this);

  while (!objects_to_delete.empty()) {
    auto current_obj = objects_to_delete.front();
    objects_to_delete.pop();

    // 获取所有的子对象，添加到队列中
    std::for_each(std::begin(current_obj->_children),
                  std::end(current_obj->_children),
                  [&](auto child) { objects_to_delete.push(child); });

    // 添加到mainWorld的待删除队列中
    mainWorld._GameObjects_to_delete.insert(current_obj);
  }
}