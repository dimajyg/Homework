#include <algorithm>
#include <cstddef>
#include <memory>
#include <tuple>

template<typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
private:
    std::tuple<T*, Deleter> unptr;

    T* ptrG() const {
        return std::get<0>(unptr);
    }
    Deleter dltrG() const {
        return std::get<1>(unptr);
    }
    T*& ptrG() {
        return std::get<0>(unptr);
    }
    Deleter& dltrG() {
        return std::get<1>(unptr);
    }

public:
    UniquePtr(T* ptr = nullptr, const Deleter& deleter = Deleter()) noexcept
    : unptr(std::make_tuple(ptr, deleter)) {}
    UniquePtr(UniquePtr&& other) noexcept : unptr(other.unptr) {
        other.ptrG() = nullptr;
    }
    ~UniquePtr() {
        dltrG()(ptrG());
    }
    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr& operator=(const UniquePtr& other) = delete;
    UniquePtr& operator=(std::nullptr_t) noexcept {
        dltrG()(ptrG());
        ptrG() = nullptr;
        return *this;
    }
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (ptrG() != other.ptrG()) {
            dltrG()(ptrG());
            unptr = other.unptr;
            other.ptrG() = nullptr;
        }
        return *this;
    }
    T& operator*() const noexcept {
        return *ptrG();
    }
    T* operator->() const noexcept {
        return ptrG();
    }
    T* release() noexcept {
        T* old = ptrG();
        ptrG() = nullptr;
        return old;
    }
    void reset(T* ptr) noexcept {
        if (ptrG() != ptr) {
            dltrG()(ptrG());
            ptrG() = ptr;
        }
    }
    void swap(UniquePtr& other) noexcept {
        std::swap(unptr, other.unptr);
    }
    T* get() const noexcept {
        return ptrG();
    }
    explicit operator bool() const noexcept {
        return (ptrG() != nullptr);
    }
    Deleter& get_deleter() noexcept {
        return dltrG();
    }
    const Deleter& get_deleter() const noexcept {
        return dltrG();
    }
};