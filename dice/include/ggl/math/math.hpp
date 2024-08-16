#ifndef MATH_H
#define MATH_H

#include <concepts>
#include <type_traits>

namespace ggl {

template<typename T>
struct is_allowed_type : std::disjunction<
    std::is_same<T, int>,
    std::is_same<T, float>,
    std::is_same<T, double>
> {};

const double PI = 3.141592653589793238462643383279502884;

template<typename T>
constexpr typename std::enable_if<is_allowed_type<T>::value, float>::type radians_to_degrees(T radians) {
    return static_cast<float>(radians) * (180/PI);
}

template<typename T>
constexpr typename std::enable_if<is_allowed_type<T>::value, float>::type degrees_to_radians(T degrees) {
    return static_cast<float>(degrees) * (PI/180);
}

template<typename T>
constexpr typename std::enable_if<is_allowed_type<T>::value, float>::type get_aspect_ratio(T width, T height) {
    return static_cast<float>(width) / static_cast<float>(height);
}

template<typename T>
constexpr T abs(T val){
    if(val >= 0) return val;
    return val * -1;
}

template<typename T>
double inline asmsqrt(T val){ // just for fun, use std::sqrt instead since it's faster
    double x;
    double v = static_cast<double>(val);

    __asm__ __inline__(
        "fsqrt"
        : "=t"(x)
        : "0"(v)
    );
    return x;
}
}//namespace ggl

#endif //MATH_H
