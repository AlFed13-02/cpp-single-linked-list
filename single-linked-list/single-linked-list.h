#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <algorithm>

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
    
    
    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node)
            :node_(node) {
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept
            :node_(other.node_) {
        }

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        BasicIterator& operator++() noexcept {
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
            return &(node_->value);
        }

    private:
        Node* node_ = nullptr;
    };

public:
    SingleLinkedList() 
        :head_(Node())
        ,size_(0) {
    }
    
    SingleLinkedList(std::initializer_list<Type> values) {
        for (auto it = std::rbegin(values); it != std::rend(values); ++it) {
            PushFront(*it);
        }
        size_ = values.size();
    }

    SingleLinkedList(const SingleLinkedList& other) {
        SingleLinkedList tmp;
        auto prev_node_ptr = &tmp.head_;
        for (auto& value: other) {
            auto next_node_ptr = new Node(value, nullptr);
            prev_node_ptr->next_node = next_node_ptr;
            prev_node_ptr = next_node_ptr;
        }
        this->swap(tmp);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (!(this == &rhs)) {
            SingleLinkedList tmp(rhs);
            this->swap(tmp);
        }
        return *this;
    }
    
    ~SingleLinkedList() {
        Clear();
    }
    
    void swap(SingleLinkedList& other) noexcept {
        auto tmp_head_next_node = this->head_.next_node;
        this->head_.next_node = other.head_.next_node;
        other.head_.next_node = tmp_head_next_node;
        
        auto tmp_size = this->size_;
        this->size_ = other.size_;
        other.size_ = tmp_size;
    }
    
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }
    
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
    
    void Clear() {
        Node* marked_for_deletion = head_.next_node;
        while (marked_for_deletion) {
            head_.next_node = head_.next_node->next_node;
            delete marked_for_deletion;
            marked_for_deletion = head_.next_node;
        }
        size_ = 0;
    }
    
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }
    
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_));
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(&head_);
    }
    
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        auto node_ptr = new Node(value, pos.node_->next_node);
        pos.node_->next_node = node_ptr;
        ++size_;
        return Iterator(node_ptr);
    }

    void PopFront() noexcept {
        auto first_node = head_.next_node;
        head_.next_node = first_node->next_node;
        --size_;
        delete first_node;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        auto marked_for_deletion = pos.node_->next_node;
        pos.node_->next_node = marked_for_deletion->next_node;
        delete marked_for_deletion;
        --size_;
        return Iterator(pos.node_->next_node);
    }

private:
    Node head_;
    size_t size_;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
} 
