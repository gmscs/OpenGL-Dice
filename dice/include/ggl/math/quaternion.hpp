#ifndef QUATERNION_H
#define QUATERNION_H

#include <concepts>
#include <iostream>
#include <cassert>
#include "vector.hpp"
#include "matrix.hpp"

namespace ggl {
template<std::floating_point T>
struct quaternion {
    T w;
    T x;
    T y;
    T z;
    
    constexpr quaternion(){
        this->w = 1;
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
    constexpr quaternion(quaternion const& q){
        this->w = w;
        this->x = x;
        this->y = y;
        this->z = z;
    }
    constexpr quaternion(vector<T> const& v){
        this->w = 1;
        this->x = v.x;
        this->y = 0;
        this->z = 0;
    }
    constexpr quaternion(vector2<T> const& v){
        this->w = 1;
        this->x = v.x;
        this->y = v.y;
        this->z = 0;
    }
    constexpr quaternion(vector3<T> const& v){
        this->w = 1;
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }
    constexpr quaternion(vector4<T> const& v){
        this->w = v.w;
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }
    explicit constexpr quaternion(T w, T x, T y, T z);
    
    constexpr quaternion<T>& operator=(quaternion<T> const &q){
        this->w = q.w;
        this->x = q.x;
        this->y = q.y;
        this->z = q.z;
        return *this;
    }
    constexpr quaternion<T>& operator+=(quaternion<T> const &q){
        this->w += q.w;
        this->x += q.x;
        this->y += q.y;
        this->z += q.z;
        return *this;
    }
    constexpr quaternion<T>& operator-=(quaternion<T> const &q){
        this->w -= q.w;
        this->x -= q.x;
        this->y -= q.y;
        this->z -= q.z;
        return *this;
    }
    constexpr quaternion<T>& operator*=(quaternion<T> const &q){
        quaternion<T> new_quat(this->w, this->x, this->y, this->z);
        new_quat = new_quat * q;
        this->w = new_quat.w;
        this->x = new_quat.x;
        this->y = new_quat.y;
        this->z = new_quat.z;
        return *this;
    }
    constexpr quaternion<T>& operator/=(quaternion<T> const &q){
        quaternion<T> new_quat(this->w, this->x, this->y, this->z);
        new_quat = new_quat / q;
        this->w = new_quat.w;
        this->x = new_quat.x;
        this->y = new_quat.y;
        this->z = new_quat.z;
        return *this;
    }
    constexpr quaternion<T>& operator*=(T c){
        this->w *= c;
        this->x *= c;
        this->y *= c;
        this->z *= c;
        return *this;
    }
    constexpr quaternion<T>& operator/=(T c){
        this->w /= c;
        this->x /= c;
        this->y /= c;
        this->z /= c;
        return *this;
    }
    
    constexpr T const& operator[](int i) const{
        assert((void("index cannot be less than 0"), i >= 0));
        assert((void("index exceeds vector size"), i < 4));
        if(i == 0) return this->w;
        else if(i == 1) return this->x;
        else if(i == 2) return this->y;
        return this->z;
    }
    constexpr T& operator[](int i){
        assert((void("index cannot be less than 0"), i >= 0));
        assert((void("index exceeds vector size"), i < 4));
        if(i == 0) return this->w;
        else if(i == 1) return this->x;
        else if(i == 2) return this->y;
        return this->z;
    }
};

template<std::floating_point T>
quaternion<T> con(quaternion<T> const& q) {
    quaternion<T> new_quat(q.w, -q.x, -q.y, -q.z);
    return new_quat;
}

template<std::floating_point T>
constexpr quaternion<T> inverse(quaternion<T> const& q){
    T magn = mag(q);
    quaternion<T> new_quat = (con(q) / (magn * magn));
    return new_quat; 
}

template<std::floating_point T>
constexpr quaternion<T> norm(quaternion<T> const& q){
    return q / mag(q);
}

template<std::floating_point T>
constexpr T mag(quaternion<T> const& q){
    return sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z); 
}

template<std::floating_point T>
constexpr quaternion<T> operator+(quaternion<T> const &q, quaternion<T> const &q2){
    quaternion<T> new_quat(q.w + q2.w, q.x + q2.x, q.y + q2.y, q.z + q2.z);
    return new_quat;
}

template<std::floating_point T>
constexpr quaternion<T> operator-(quaternion<T> const &q, quaternion<T> const &q2){
    quaternion<T> new_quat(q.w - q2.w, q.x - q2.x, q.y - q2.y, q.z - q2.z);
    return new_quat;
}

template<std::floating_point T>
constexpr quaternion<T> operator*(quaternion<T> const &q, quaternion<T> const &q2){
    quaternion<T> new_quat;
    new_quat.w = (q.w * q2.w) - (q.x * q2.x) - (q.y * q2.y) - (q.z * q2.z);
    new_quat.x = (q.w * q2.x) + (q.x * q2.w) + (q.y * q2.z) - (q.z * q2.y);
    new_quat.y = (q.w * q2.y) - (q.x * q2.z) + (q.y * q2.w) + (q.z * q2.x);
    new_quat.z = (q.w * q2.z) + (q.x * q2.y) - (q.y * q2.x) + (q.z * q2.w);
    return new_quat;
}

template<std::floating_point T>
constexpr quaternion<T> operator*(quaternion<T> const& q, T c){
    quaternion<T> new_quat(q.w * c, q.x * c, q.y * c, q.z * c);
    return new_quat;
}

template<std::floating_point T>
constexpr quaternion<T> operator*(T c, quaternion<T> const& q){
    quaternion<T> new_quat(q.w * c, q.x * c, q.y * c, q.z * c);
    return new_quat;
}

template<std::floating_point T>
constexpr quaternion<T> operator/(quaternion<T> const& q, quaternion<T> const &q2){
    quaternion<T> new_quat = q * inverse(q2);
    return new_quat;
} 

template<std::floating_point T>
constexpr quaternion<T> operator/(quaternion<T> const& q, T c){
    quaternion<T> new_quat(q.w / c, q.x / c, q.y / c, q.z / c);
    return new_quat;
}

template<std::floating_point T>
constexpr quaternion<T> operator/(T c, quaternion<T> const& q){
    quaternion<T> new_quat(q.w / c, q.x / c, q.y / c, q.z / c);
    return new_quat;
}

template<std::floating_point T>
constexpr matrix4<T> quatToMatrix(quaternion<T> const& q){
    matrix4<T> new_mat(1);
    
    new_mat[0][0] = 1 - 2 * (q.y * q.y) - 2 * (q.z * q.z);
    new_mat[0][1] = 2 * (q.x * q.y) - 2 * (q.z * q.w);
    new_mat[0][2] = 2 * (q.x * q.z) + 2 * (q.y * q.w);
    
    new_mat[1][0] = 2 * (q.x * q.y) + 2 * (q.z * q.w);
    new_mat[1][1] = 1 - 2 * (q.x * q.x) - 2 * (q.z * q.z);
    new_mat[1][2] = 2 * (q.y * q.z) - 2 * (q.x * q.w);
    
    new_mat[2][0] = 2 * (q.x * q.z) - 2 * (q.y * q.w);
    new_mat[2][1] = 2 * (q.y * q.z) + 2 * (q.x * q.w);
    new_mat[2][2] = 1 - 2 * (q.x * q.x) - 2 * (q.y * q.y);
    
    return new_mat;
}

/* Printing */
template<std::floating_point T>
void print_quat(quaternion<T> const& q){
    std::cout << "{" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << "}\n";
}

}

#endif //QUATERNION_H