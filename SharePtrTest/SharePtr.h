template <typename T>
class SharedPtr {
public:
    SharedPtr(T* ptr = nullptr) : _ptr(ptr), _count(ptr ? new int(1) : nullptr) {}
    
    ~SharedPtr() { release(); }

    SharedPtr(const SharedPtr& other) : _ptr(other._ptr), _count(other._count) {
        if (_count) (*_count)++;
    }

    //注意入参
    SharedPtr& operator=(SharedPtr other) {
        std::swap(_ptr, other._ptr);
        std::swap(_count, other._count);
        return *this;
    }

    T& operator*() { return *_ptr; }
    T* operator->() { return _ptr; }

private:
    void release() {
        if (_count && --(*_count) == 0) {
            delete _ptr;
            delete _count;
        }
    }
    T* _ptr;
    int* _count; // 共享的计数器
};