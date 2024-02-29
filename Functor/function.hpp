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

    Function(const Function& other) {
        if (!other.functor_ptr) throw std::invalid_argument("Functor ptr was NULL!");
        functor_ptr = other.functor_ptr->createCopy();
    }

    Function& operator=(const Function& other) {
        if (!other.functor_ptr) throw std::invalid_argument("Functor ptr was NULL!");

        delete functor_ptr;
        functor_ptr = other.functor_ptr->createCopy();

        return *this;
    }

    Function(Function&& other) noexcept
        : functor_ptr(other.functor_ptr) {
        other.functor_ptr = nullptr;
    }

    Function& operator=(Function&& other) noexcept {
        if (this != &other) {
            delete functor_ptr;

            functor_ptr = other.functor_ptr;
            other.functor_ptr = nullptr;
        }

        return *this;
    }

    Return operator()(Args... args) const {
        if (!functor_ptr) throw std::invalid_argument("Functor ptr was NULL!");
        return (*functor_ptr)(args...);
    }

    operator bool() const noexcept {
        return functor_ptr;
    }

    const std::type_info& targetType() const {
        if (!functor_ptr) throw std::invalid_argument("Functor ptr was NULL!");
        return functor_ptr->targetType();
    }

    template<typename U>
    const U* target() const {
        return static_cast<Functor<U>>(functor_ptr).getTarget();
    }

    ~Function() {
        delete functor_ptr;
    }

private:
    struct IFunctor {
        virtual Return                operator()(Args... args) const          = 0;
        virtual const std::type_info& targetType()             const noexcept = 0;
        virtual                      ~IFunctor  ()                            = default;

        virtual IFunctor* createCopy() const = 0;
    };

    template<typename T>
    class Functor : public IFunctor {
    public:
        Functor(T _target) 
            : target(_target) {
        }

        Functor(const Functor& other)
            : target(other.target) {
        }

        Functor& operator=(const Functor& other) {
            target = other.target;
            return *this;
        }

        Functor(Functor&& other)
            : target(std::move(other.target)) {
        }

        Functor& operator=(Functor&& other) {
            target = std::move(other.target);
            return *this;
        }

        Return operator()(Args... args) const override {
            return target(args...);
        }

        IFunctor* createCopy() const override {
            return new Functor<T>(target);
        }

        const std::type_info& targetType() const noexcept override {
            return typeid(target);
        }

        const T* getTarget() const noexcept {
            return &target;
        }

    private:
        T target;
    };

private:
    IFunctor* functor_ptr;
};

#endif