#include <iostream>
#include <functional>

template<typename R, typename A1, typename A2> 
auto curry(R (*f) (A1, A2)) 
{
    return [=](A1 a1_){
        return [=](A2 a2_){
            return f(a1_, a2_);
        };
    };
}

template<typename R, typename A1, typename A2, typename A3> 
auto curry(R (*f) (A1, A2, A3)) 
{
    return [=](A1 a1_){
        return [=](A2 a2_){
            return [=](A3 a3_){
                return f(a1_, a2_, a3_);
            };
        };
    };
}

template<typename R, typename A1, typename A2, typename A3, typename A4> 
auto curry(R (*f) (A1, A2, A3, A4)) 
{
    return [=](A1 a1_){
        return [=](A2 a2_){
            return [=](A3 a3_){
                return [=](A4 a4_){
                    return f(a1_, a2_, a3_, a4_);
                };
            };
        };
    };
}

template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5> 
auto curry(R (*f) (A1, A2, A3, A4, A5)) 
{
    return [=](A1 a1_){
        return [=](A2 a2_){
            return [=](A3 a3_){
                return [=](A4 a4_){
                    return [=](A5 a5_){
                        return f(a1_, a2_, a3_, a4_, a5_);
                    };
                };
            };
        };
    };
}


template<typename R, typename A1, typename A2, typename... As> 
struct Curry_
{
    using F = std::function<R(A1, A2, As...)>;
    
    Curry_( F f )
        :f_(f){};
    
    auto operator()(A1 a1) const
    {
        return Curry_R<F, A2, As...>(f_, a1);
    };

private:
    F f_;

protected:
    template <typename F_, typename A2_, typename... As_>
    struct Curry_R
    {
        Curry_R( F_ f, A1 a1_ )
            :f_(f){};
        
        auto operator()(A2_ a2) const
        {
            return Curry_R<F, As...>(f_, a2);
        };
    
    private:
        F f_;
        A1 a1_;
    };

};

template<typename R, typename A1, typename A2, typename... As>
auto curry_( R (*f) (A1, A2, As...) )
{
    return Curry_<R, A1, A2, As...>(f);
}

// test functions
int add2 (int a, int& b)
{
    return a+b;
}

int add3 (int a, int b, int c)
{
    return a+b+c;
}

int add4 (int a, int b, int c, int d)
{
    return a+b+c+d;
}

int add5 (double a, int b, float c, long d, int e)
{
    return a+b+c+d+e;
}

// main function
int main()
{
    auto f2 = curry(add2);
    auto f3 = curry(add3);
    auto f4 = curry(add4);
    int a = 10;
    int b = 20;
    std::cout << f2(a)(b) << std::endl;
    std::cout << f3(30)(20)(10) << std::endl;
    std::cout << f4(40)(30)(20)(10) << std::endl;
    auto ff = curry_(add2);
}
