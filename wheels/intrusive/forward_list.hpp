#pragma once

#include <wheels/support/assert.hpp>

namespace wheels {

template <typename T>
struct IntrusiveForwardListNode {
  // Shortcut
  using Node = IntrusiveForwardListNode;

  Node* next_;

  void SetNext(Node* node) noexcept {
    next_ = node;
  }

  void ResetNext() noexcept {
    SetNext(nullptr);
  }

  void LinkBefore(Node* node) noexcept {
    SetNext(node);
  }

  T* AsItem() noexcept {
    return static_cast<T*>(this);
  }
};

template <typename T>
class IntrusiveForwardList {
  // Node type
  using Node = IntrusiveForwardListNode<T>;

 public:
  void PushBack(Node* node) noexcept {
    ++size_;
    if (IsEmpty()) {
      head_ = tail_ = node;
    } else {
      tail_->next_ = node;
      tail_ = node;
    }
  }

  void PushFront(Node* node) noexcept {
    ++size_;
    if (IsEmpty()) {
      head_ = tail_ = node;
    } else {
      node->next_ = head_;
      head_= node;
    }
  }

  T* PopFront() noexcept {
    if (IsEmpty()) {
      return nullptr;
    }

    --size_;
    Node* front = head_;

    if (head_ == tail_) {
      head_ = tail_ = nullptr;
    } else {
      head_ = head_->next_;
    }

    front->next_ = nullptr;

    return front->AsItem();
  }

  bool IsEmpty() const noexcept {
    return head_ == nullptr;
  }

  // O(1) worst case
  size_t Size() const noexcept {
    return size_;
  }

  void Reset() noexcept {
    head_ = tail_ = nullptr;
    size_ = 0;
  }

  // TODO: Iterators

  ~IntrusiveForwardList() {
    WHEELS_ASSERT(IsEmpty(), "List is not empty");
  }

 private:
  Node* head_ = nullptr;
  Node* tail_ = nullptr;
  size_t size_ = 0;
};

}  // namespace wheels
