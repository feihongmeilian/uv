#ifndef NONCOPYABLE_HPP
#define NONCOPYABLE_HPP

class Noncopyable
{
protected:
    Noncopyable() = default;
    ~Noncopyable() = default;

private:
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};

#endif
