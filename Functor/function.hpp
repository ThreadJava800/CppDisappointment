#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <stdexcept>

template<typename>
class Function;

template<typename Return, typename... Args>
class Function<Return(Args...)> {
public:
    explicit Function() {
    }

    template<typename U>
    explicit Function(U functor) 
        : functor_ptr(new Functor<U>(functor)) {
    }

    Return operator()(Args... args) const {
        if (!functor_ptr) throw std::invalid_argument("Functor ptr was NULL!");
        return (*functor_ptr)(args...);
    }

    ~Function() {
        delete functor_ptr;
    }

private:
    struct IFunctor {
        virtual Return operator()(Args... args) const = 0;
        virtual ~IFunctor()                           = default;
    };

    template<typename T>
    class Functor : public IFunctor {
    public:
        Functor(T _target) 
            : target(_target) {
        }

        Return operator()(Args... args) const override {
            return target(args...);
        }

    private:
        T target;
    };

private:
    IFunctor* functor_ptr;
};

#endif