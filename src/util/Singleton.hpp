#pragma once

namespace fi::util {

template <typename T>
class Singleton {
public:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;

    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static T& get() {
        static T instance;
        return instance;
    }

private:
    friend T;

    Singleton() = default;
    virtual ~Singleton() = default;
};

} // namespace fi::util