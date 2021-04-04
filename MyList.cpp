#include <utility>
#include <algorithm>
template<typename T>
class List {
private:
    struct Node {
        Node* prev_;
        Node* next_;
        Node() {
            prev_ = next_ = this;
        }
        Node(Node* prev, Node* next) : prev_(prev), next_(next) {}
        virtual ~Node() {}
    };
    struct ValueNode : public Node {
        T value_;
        ValueNode(const T& value, Node* prev = nullptr, Node* next = nullptr)
            : Node(prev, next), value_(value) {}
        ValueNode(T&& value, Node* prev = nullptr, Node* next = nullptr)
            : Node(prev, next), value_(std::move(value)) {}
    };
    size_t size_;
    Node* f_el;

public:
    class Iterator {
    private:
        Node* itr;

    public:
        Iterator(Node* ptr) : itr(ptr) {}
        Iterator(const Iterator& other) : itr(other.itr) {}
        Iterator(Iterator&& other) : itr(std::move(other.itr)) {}
        Iterator& operator++() {
            itr = (*itr).next_;
            return *this;
        }
        Iterator operator++(int) {
            Iterator old(*this);
            itr = (*itr).next_;
            return old;
        }
        Iterator& operator--() {
            itr = (*itr).prev_;
            return *this;
        }
        Iterator operator--(int) {
            Iterator old(*this);
            itr = (*itr).prev_;
            return old;
        }
        T& operator*() const {
            return (*static_cast<ValueNode*>(itr)).value_;
        }
        T* operator->() const {
            return &((*static_cast<ValueNode*>(itr)).value_);
        }
        bool operator==(const Iterator& other) const {
            return itr == other.itr;
        }
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };
    List() : size_(), f_el(new Node()) {}
    List(const List& other) : size_(other.size_), f_el(new Node(other.f_el)) {
        auto left = f_el;
        auto right = other.f_el;
        while (right->next_ != right) {
            auto v = static_cast<ValueNode*>(right->next_)->value_;
            left->next_ = new ValueNode(std::move(v));
            right = right->next_;
            left = left->next_;
        }
        f_el->prev_ = left;
        left->next_ = f_el;
    }
    List(List&& other) : size_(other.size_), f_el(other.f_el) {
        other.f_el = nullptr;
        other.size_ = 0;
    }
    ~List() {
        if (size_ != 0) {
            auto cur = f_el->next_;
            while (cur != f_el) {
                auto next = cur->next_;
                delete cur;
                cur = next;
            }
        }
        delete f_el;
    }
    List& operator=(const List& other) {
        List old(other);
        swap(old);
        return *this;
    }
    List& operator=(List&& other) {
        List old(std::move(other));
        swap(old);
        return *this;
    }
    bool empty() const {
        return size_ == 0;
    }
    size_t size() const {
        return size_;
    }
    void swap(List& other) {
        std::swap(f_el, other.f_el);
        std::swap(size_, other.size_);
    }
    void Unlink(Node* i) {
        i->prev_->next_ = i->next_;
        i->next_->prev_ = i->prev_;
        i->next_ = i->prev_ = nullptr;
        delete i;
    }
    void LinkAfter(Node* i, Node* after) {
        after->next_ = i->next_;
        after->prev_ = i;
        after->next_->prev_ = after;
        i->next_ = after;
    }
    void push_back(const T& elem) {
        Node* new_node = new ValueNode(elem);
        LinkAfter(f_el->prev_, new_node);
        size_++;
    }
    void push_back(T&& elem) {
        Node* new_node = new ValueNode(std::move(elem));
        LinkAfter(f_el->prev_, new_node);
        size_++;
    }
    void push_front(const T& elem) {
        Node* new_node = new ValueNode(elem);
        LinkAfter(f_el, new_node);
        size_++;
    }
    void push_front(T&& elem) {
        Node* new_node = new ValueNode(std::move(elem));
        LinkAfter(f_el, new_node);
        size_++;
    }
    T& back() {
        return static_cast<ValueNode*>(f_el->prev_)->value_;
    }
    const T& back() const {
        return static_cast<ValueNode*>(f_el->prev_)->value_;
    }
    T& front() {
        return static_cast<ValueNode*>(f_el->next_)->value_;
    }
    const T& front() const {
        return static_cast<ValueNode*>(f_el->next_)->value_;
    }
    void pop_back() {
        Unlink(f_el->prev_);
        size_--;
    }
    void pop_front() {
        Unlink(f_el->next_);
        size_--;
    }
    Iterator begin() {
        return Iterator(f_el->next_);
    }
    Iterator end() {
        return Iterator(f_el);
    }
};
template <typename T>
typename List<T>::Iterator begin(List<T>& list) {
    return list.begin();
}
template <typename T>
typename List<T>::Iterator end(List<T>& list) {
    return list.end();
}