#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <algorithm>
#include <cassert>

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
			assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        [[nodiscard]] reference operator*() const noexcept {
			assert(node_ != nullptr);
            return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
			assert(node_ != nullptr);
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

    template <typename Values>
	void Init(const Values& values) {
        SingleLinkedList tmp;
        auto prev_node_ptr = &tmp.head_;
        for (auto& value: values) {
            auto next_node_ptr = new Node(value, nullptr);
            prev_node_ptr->next_node = next_node_ptr;
            prev_node_ptr = next_node_ptr;
            ++tmp.size_;
        }
        swap(tmp);
        
    }

	SingleLinkedList(std::initializer_list<Type> values) {
        Init(values);
    }

	SingleLinkedList(const SingleLinkedList& other) {
		Init(other);
	}

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (!(this == &rhs)) {
            SingleLinkedList tmp(rhs);
            swap(tmp);
        }
        return *this;
    }
    
    ~SingleLinkedList() {
        Clear();
    }
    
    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
		std::swap(size_, other.size_);
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
        return begin();
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return end();
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
		assert(pos.node_ != nullptr);
        auto node_ptr = new Node(value, pos.node_->next_node);
        pos.node_->next_node = node_ptr;
        ++size_;
        return Iterator(node_ptr);
    }

    void PopFront() noexcept {
		assert(!IsEmpty());
        auto first_node = head_.next_node;
        head_.next_node = first_node->next_node;
        --size_;
        delete first_node;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
		assert(size_ != 0 && pos.node_ != nullptr && pos.node_->next_node != nullptr);
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
    if (&lhs == &rhs) {
        return true;
    }
    if (lhs.GetSize() == rhs.GetSize()) {
	    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
    return false;
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