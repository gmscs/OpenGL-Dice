#ifndef MATRIX_H
#define MATRIX_H

#include <concepts>
#include <math.h>
#include <cassert>
#include <typeinfo>
#include <iostream>
#include "vector.hpp"

namespace ggl {
template<std::floating_point T>
struct matrix2 {
    vector2<T> row1;
    vector2<T> row2;

    constexpr matrix2(){
        this->row1 = vector2<T>();
        this->row2 = vector2<T>();
    }
    constexpr matrix2(T c){
        this->row1 = vector2<T>(c, 0);
        this->row2 = vector2<T>(0, c);
    }
    constexpr matrix2(matrix2 const& m){
        this->row1 = vector2<T>(m.row1);
        this->row2 = vector2<T>(m.row2);
    }
    explicit constexpr matrix2(vector2<T> const &a, vector2<T> const &b){
        this->row1 = a;
        this->row2 = b;
    }

    static inline constexpr int numRows() {
        return 2;
    }

    static inline constexpr int numCols() {
        return 2;
    }

    static inline constexpr const char* type() {
        return typeid(T).name();
    }

    template<std::floating_point N>
    constexpr matrix2<T>& operator=(matrix2<N> const& m){
        this->row1 = m.row1;
        this->row2 = m.row2;
        return *this;
    }
    template<std::floating_point N>
        constexpr matrix2<T>& operator+=(matrix2<N> const& m){
        this->row1 += m.row1;
        this->row2 += m.row2;
        return *this;
    }
    template<std::floating_point N>
    constexpr matrix2<T>& operator-=(matrix2<N> const& m){
        this->row1 -= m.row1;
        this->row2 -= m.row2;
        return *this;
    }
    // template<std::floating_point N>
    // constexpr matrix2<T>& operator*=(matrix2<N> const& m);
    template<typename N>
    constexpr matrix2<T>& operator*=(N c){
        this->row1 *= c;
        this->row2 *= c;
        return *this;
    }
    template<typename N>
    constexpr matrix2<T>& operator/=(N c){
        this->row2 /= c;
        this->row1 /= c;
        return *this;
    }
    constexpr vector2<T> const& operator[](int i) const{
        assert((void("index cannot be less than 0"), i >= 0));
        assert((void("index exceeds vector size"), i < this->numRows()));
        if(i == 0) return this->row1;
        return this->row2;
    }
    constexpr vector2<T>& operator[](int i){
        assert((void("index cannot be less than 0"), i >= 0));
        assert((void("index exceeds vector size"), i < this->numRows()));
        if(i == 0) return this->row1;
        return this->row2;
    }
};

template<std::floating_point T>
struct matrix3 {
    vector3<T> row1;
    vector3<T> row2;
    vector3<T> row3;

    constexpr matrix3(){
        this->row1 = vector3<T>();
        this->row2 = vector3<T>();
        this->row3 = vector3<T>();
    }
    constexpr matrix3(T c){
        this->row1 = vector3<T>(c, 0, 0);
        this->row2 = vector3<T>(0, c, 0);
        this->row3 = vector3<T>(0, 0, c);
    }
    constexpr matrix3(matrix3 const& m){
        this->row1 = vector3<T>(m.row1);
        this->row2 = vector3<T>(m.row2);
        this->row3 = vector3<T>(m.row3);
    }
    explicit constexpr matrix3(vector3<T> const &a, vector3<T> const &b, vector3<T> const &c){
        this->row1 = a;
        this->row2 = b;
        this->row3 = c;
    }

    static inline constexpr int numRows() {
        return 3;
    }

    static inline constexpr int numCols() {
        return 3;
    }

    static inline constexpr const char* type() {
        return typeid(T).name();
    }

    template<std::floating_point N>
    constexpr matrix3<T>& operator=(matrix3<N> const& m){
        this->row1 = m.row1;
        this->row2 = m.row2;
        this->row3 = m.row3;
        return *this;
    }
    template<std::floating_point N>
    constexpr matrix3<T>& operator+=(matrix3<N> const& m){
        this->row1 += m.row1;
        this->row2 += m.row2;
        this->row3 += m.row3;
        return *this;
    }
    template<std::floating_point N>
    constexpr matrix3<T>& operator-=(matrix3<N> const& m){
        this->row1 -= m.row1;
        this->row2 -= m.row2;
        this->row3 -= m.row3;
        return *this;
    }
    // template<std::floating_point N>
    // constexpr matrix3<T>& operator*=(matrix3<N> const& m);
    template<typename N>
    constexpr matrix3<T>& operator*=(N c){
        this->row1 *= c;
        this->row2 *= c;
        this->row3 *= c;
        return *this;
    }
    template<typename N>
    constexpr matrix3<T>& operator/=(N c){
        this->row1 /= c;
        this->row2 /= c;
        this->row3 /= c;
        return *this;
    }
    constexpr vector3<T> const& operator[](int i) const{
        assert((void("index cannot be less than 0"), i >= 0));
        assert((void("index exceeds vector size"), i < this->numRows()));
        if(i == 0) return this->row1;
        else if(i == 1) return this->row2;
        return this->row3;
    }
    constexpr vector3<T>& operator[](int i){
        assert((void("index cannot be less than 0"), i >= 0));
        assert((void("index exceeds vector size"), i < this->numRows()));
        if(i == 0) return this->row1;
        else if(i == 1) return this->row2;
        return this->row3;
    }
};

template<std::floating_point T>
struct matrix4 {
    vector4<T> row1;
    vector4<T> row2;
    vector4<T> row3;
    vector4<T> row4;

    constexpr matrix4(){
        this->row1 = vector4<T>();
        this->row2 = vector4<T>();
        this->row3 = vector4<T>();
        this->row4 = vector4<T>();
    }
    constexpr matrix4(T c){
        this->row1 = vector4<T>(c, 0, 0, 0);
        this->row2 = vector4<T>(0, c, 0, 0);
        this->row3 = vector4<T>(0, 0, c, 0);
        this->row4 = vector4<T>(0, 0, 0, c);
    }
    constexpr matrix4(matrix4 const& m){
        this->row1 = vector4<T>(m.row1);
        this->row2 = vector4<T>(m.row2);
        this->row3 = vector4<T>(m.row3);
        this->row4 = vector4<T>(m.row4);
    }
    explicit constexpr matrix4(vector4<T> const &a, vector4<T> const &b, vector4<T> const &c, vector4<T> const &d){
        this->row1 = a;
        this->row2 = b;
        this->row3 = c;
        this->row4 = d;
    }

    static inline constexpr int numRows() {
        return 4;
    }

    static inline constexpr int numCols() {
        return 4;
    }

    static inline constexpr const char* type() {
        return typeid(T).name();
    }

    template<std::floating_point N>
    constexpr matrix4<T>& operator=(matrix4<N> const& m){
        this->row1 = m.row1;
        this->row2 = m.row2;
        this->row3 = m.row3;
        this->row4 = m.row4;
        return *this;
    }
    template<std::floating_point N>
    constexpr matrix4<T>& operator+=(matrix4<N> const& m){
        this->row1 += m.row1;
        this->row2 += m.row2;
        this->row3 += m.row3;
        this->row4 += m.row4;
        return *this;
    }
    template<std::floating_point N>
    constexpr matrix4<T>& operator-=(matrix4<N> const& m){
        this->row1 -= m.row1;
        this->row2 -= m.row2;
        this->row3 -= m.row3;
        this->row4 -= m.row4;
        return *this;
    }
    // template<std::floating_point N>
    // constexpr matrix4<T>& operator*=(matrix4<N> const& m);
    template<typename N>
    constexpr matrix4<T>& operator*=(N c){
        this->row1 *= c;
        this->row2 *= c;
        this->row3 *= c;
        this->row4 *= c;
        return *this;
    }
    template<typename N>
    constexpr matrix4<T>& operator/=(N c){
        this->row1 /= c;
        this->row2 /= c;
        this->row3 /= c;
        this->row4 /= c;
        return *this;
    }
    constexpr vector4<T> const& operator[](int i) const{
        assert((void("index cannot be less than 0"), i >= 0));
        assert((void("index exceeds vector size"), i < this->numRows()));
        if(i == 0) return this->row1;
        else if(i == 1) return this->row2;
        else if(i == 2) return this->row3;
        return this->row4;
    }
    constexpr vector4<T>& operator[](int i){
        assert((void("index cannot be less than 0"), i >= 0));
        assert((void("index exceeds vector size"), i < this->numRows()));
        if(i == 0) return this->row1;
        else if(i == 1) return this->row2;
        else if(i == 2) return this->row3;
        return this->row4;
    }
};

/* Sign flip */
template<std::floating_point T>
constexpr matrix2<T> operator-(matrix2<T> const &m) {
    matrix2<T> new_mat(-m.row1, -m.row2);
    return new_mat;
}

template<std::floating_point T>
constexpr matrix3<T> operator-(matrix3<T> const &m) {
    matrix3<T> new_mat(-m.row1, -m.row2, -m.row3);
    return new_mat;
}

template<std::floating_point T>
constexpr matrix4<T> operator-(matrix4<T> const &m) {
    matrix4<T> new_mat(-m.row1, -m.row2, -m.row3, -m.row4);
    return new_mat;
}

/* Addition */
template<std::floating_point T>
constexpr matrix2<T> operator+(matrix2<T> const &m, matrix2<T> const &m2) {
    matrix2<T> new_mat(m.row1 + m2.row1, m.row2 + m2.row2);
    return new_mat;
}

template<std::floating_point T>
constexpr matrix3<T> operator+(matrix3<T> const &m, matrix3<T> const &m2) {
    matrix3<T> new_mat(m.row1 + m2.row1, m.row2 + m2.row2, m.row3 + m2.row3);
    return new_mat;
}

template<std::floating_point T>
constexpr matrix4<T> operator+(matrix4<T> const &m, matrix4<T> const &m2) {
    matrix4<T> new_mat(m.row1 + m2.row1, m.row2 + m2.row2, m.row3 + m2.row3, m.row4 + m2.row4);
    return new_mat;
}

/* Subtraction */
template<std::floating_point T>
constexpr matrix2<T> operator-(matrix2<T> const &m, matrix2<T> const &m2) {
    matrix2<T> new_mat(m.row1 - m2.row1, m.row2 - m2.row2);
    return new_mat;
}

template<std::floating_point T>
constexpr matrix3<T> operator-(matrix3<T> const &m, matrix3<T> const &m2) {
    matrix3<T> new_mat(m.row1 - m2.row1, m.row2 - m2.row2, m.row3 - m2.row3);
    return new_mat;
}

template<std::floating_point T>
constexpr matrix4<T> operator-(matrix4<T> const &m, matrix4<T> const &m2) {
    matrix4<T> new_mat(m.row1 - m2.row1, m.row2 - m2.row2, m.row3 - m2.row3, m.row4 - m2.row4);
    return new_mat;
}

/* Multiplication */
template<std::floating_point T>
constexpr matrix2<T> operator*(matrix2<T> const &m, matrix2<T> const &m2) {
    matrix2<T> new_mat;
    new_mat[0][0] = (m[0][0] * m2[0][0]) + (m[0][1] * m2[1][0]);
    new_mat[0][1] = (m[0][0] * m2[0][1]) + (m[0][1] * m2[1][1]);

    new_mat[1][0] = (m[1][0] * m2[0][0]) + (m[1][1] * m2[1][0]);
    new_mat[1][1] = (m[1][0] * m2[0][1]) + (m[1][1] * m2[1][1]);

    return new_mat;
}

template<std::floating_point T>
constexpr matrix3<T> operator*(matrix3<T> const &m, matrix3<T> const &m2) {
    matrix3<T> new_mat;
    new_mat[0][0] = (m[0][0] * m2[0][0]) + (m[0][1] * m2[1][0]) + (m[0][2] * m2[2][0]);
    new_mat[1][0] = (m[1][0] * m2[0][0]) + (m[1][1] * m2[1][0]) + (m[1][2] * m2[2][0]);
    new_mat[2][0] = (m[2][0] * m2[0][0]) + (m[2][1] * m2[1][0]) + (m[2][2] * m2[2][0]);

    new_mat[0][1] = (m[0][0] * m2[0][1]) + (m[0][1] * m2[1][1]) + (m[0][2] * m2[2][1]);
    new_mat[1][1] = (m[1][0] * m2[0][1]) + (m[1][1] * m2[1][1]) + (m[1][2] * m2[2][1]);
    new_mat[2][1] = (m[2][0] * m2[0][1]) + (m[2][1] * m2[1][1]) + (m[2][2] * m2[2][1]);

    new_mat[0][2] = (m[0][0] * m2[0][2]) + (m[0][1] * m2[1][2]) + (m[0][2] * m2[2][2]);
    new_mat[1][2] = (m[1][0] * m2[0][2]) + (m[1][1] * m2[1][2]) + (m[1][2] * m2[2][2]);
    new_mat[2][2] = (m[2][0] * m2[0][2]) + (m[2][1] * m2[1][2]) + (m[2][2] * m2[2][2]);

    return new_mat;
}

template<std::floating_point T>
constexpr matrix4<T> operator*(matrix4<T> const &m, matrix4<T> const &m2) {
    matrix4<T> new_mat;
    new_mat[0][0] = (m[0][0] * m2[0][0]) + (m[0][1] * m2[1][0]) + (m[0][2] * m2[2][0]) + (m[0][3] * m2[3][0]);
    new_mat[1][0] = (m[1][0] * m2[0][0]) + (m[1][1] * m2[1][0]) + (m[1][2] * m2[2][0]) + (m[1][3] * m2[3][0]);
    new_mat[2][0] = (m[2][0] * m2[0][0]) + (m[2][1] * m2[1][0]) + (m[2][2] * m2[2][0]) + (m[2][3] * m2[3][0]);
    new_mat[3][0] = (m[3][0] * m2[0][0]) + (m[3][1] * m2[1][0]) + (m[3][2] * m2[2][0]) + (m[3][3] * m2[3][0]);

    new_mat[0][1] = (m[0][0] * m2[0][1]) + (m[0][1] * m2[1][1]) + (m[0][2] * m2[2][1]) + (m[0][3] * m2[3][1]);
    new_mat[1][1] = (m[1][0] * m2[0][1]) + (m[1][1] * m2[1][1]) + (m[1][2] * m2[2][1]) + (m[1][3] * m2[3][1]);
    new_mat[2][1] = (m[2][0] * m2[0][1]) + (m[2][1] * m2[1][1]) + (m[2][2] * m2[2][1]) + (m[2][3] * m2[3][1]);
    new_mat[3][1] = (m[3][0] * m2[0][1]) + (m[3][1] * m2[1][1]) + (m[3][2] * m2[2][1]) + (m[3][3] * m2[3][1]);

    new_mat[0][2] = (m[0][0] * m2[0][2]) + (m[0][1] * m2[1][2]) + (m[0][2] * m2[2][2]) + (m[0][3] * m2[3][2]);
    new_mat[1][2] = (m[1][0] * m2[0][2]) + (m[1][1] * m2[1][2]) + (m[1][2] * m2[2][2]) + (m[1][3] * m2[3][2]);
    new_mat[2][2] = (m[2][0] * m2[0][2]) + (m[2][1] * m2[1][2]) + (m[2][2] * m2[2][2]) + (m[2][3] * m2[3][2]);
    new_mat[3][2] = (m[3][0] * m2[0][2]) + (m[3][1] * m2[1][2]) + (m[3][2] * m2[2][2]) + (m[3][3] * m2[3][2]);

    new_mat[0][3] = (m[0][0] * m2[0][3]) + (m[0][1] * m2[1][3]) + (m[0][2] * m2[2][3]) + (m[0][3] * m2[3][3]);
    new_mat[1][3] = (m[1][0] * m2[0][3]) + (m[1][1] * m2[1][3]) + (m[1][2] * m2[2][3]) + (m[1][3] * m2[3][3]);
    new_mat[2][3] = (m[2][0] * m2[0][3]) + (m[2][1] * m2[1][3]) + (m[2][2] * m2[2][3]) + (m[2][3] * m2[3][3]);
    new_mat[3][3] = (m[3][0] * m2[0][3]) + (m[3][1] * m2[1][3]) + (m[3][2] * m2[2][3]) + (m[3][3] * m2[3][3]);

    return new_mat;
}

template<std::floating_point T, typename N>
constexpr matrix2<T> operator*(matrix2<T> const &m, N c) {
    matrix2<T> new_mat(m.row1 * c, m.row2 * c);
    return new_mat;
}

template<std::floating_point T, typename N>
constexpr matrix3<T> operator*(matrix3<T> const &m, N c) {
    matrix3<T> new_mat(m.row1 * c, m.row2 * c, m.row3 * c);
    return new_mat;
}

template<std::floating_point T, typename N>
constexpr matrix4<T> operator*(matrix4<T> const &m, N c){
    matrix4<T> new_mat(m.row1 * c, m.row2 * c, m.row3 * c, m.row4 * c);
    return new_mat;
}

template<std::floating_point T, typename N>
constexpr matrix2<T> operator*(N c, matrix2<T> const &m) {
    matrix2<T> new_mat(m.row1 * c, m.row2 * c);
    return new_mat;
}

template<std::floating_point T, typename N>
constexpr matrix3<T> operator*(N c, matrix3<T> const &m) {
    matrix3<T> new_mat(m.row1 * c, m.row2 * c, m.row3 * c);
    return new_mat;
}

template<std::floating_point T, typename N>
constexpr matrix4<T> operator*(N c, matrix4<T> const &m){
    matrix4<T> new_mat(m.row1 * c, m.row2 * c, m.row3 * c, m.row4 * c);
    return new_mat;
}

template<std::floating_point T, std::floating_point N>
constexpr ggl::vector2<T> operator*(matrix2<T> const &m, ggl::vector2<N> const &v) {
    ggl::vector2<T> new_vec((m.row1.x * v.x + m.row1.y * v.y), (m.row2.x * v.x + m.row2.y * v.y));
    return new_vec;
}

template<std::floating_point T, std::floating_point N>
constexpr ggl::vector3<T> operator*(matrix3<T> const &m, ggl::vector3<N> const &v) {
    ggl::vector3<T> new_vec((m.row1.x * v.x + m.row1.y * v.y + m.row1.z * v.z), (m.row2.x * v.x + m.row2.y * v.y + m.row2.z * v.z), (m.row3.x * v.x + m.row3.y * v.y + m.row3.z * v.z));
    return new_vec;
}


template<std::floating_point T, std::floating_point N>
constexpr ggl::vector4<T> operator*(matrix4<T> const &m, ggl::vector4<N> const &v) {
    ggl::vector4<T> new_vec((m.row1.x * v.x + m.row1.y * v.y + m.row1.z * v.z + m.row1.w * v.w), (m.row2.x * v.x + m.row2.y * v.y + m.row2.z * v.z + m.row2.w * v.w), (m.row3.x * v.x + m.row3.y * v.y + m.row3.z * v.z + m.row3.w * v.w), (m.row4.x * v.x + m.row4.y * v.y + m.row4.z * v.z + m.row4.w * v.w));
    return new_vec;
}

/* Division */
template<std::floating_point T, typename N>
constexpr matrix2<T> operator/(matrix2<T> const &m, N c) {
    matrix2<T> new_mat(m.row1 / c, m.row2 / c);
    return new_mat;
}

template<std::floating_point T, typename N>
constexpr matrix3<T> operator/(matrix3<T> const &m, N c) {
    matrix3<T> new_mat(m.row1 / c, m.row2 / c, m.row3 / c);
    return new_mat;
}

template<std::floating_point T, typename N>
constexpr matrix4<T> operator/(matrix4<T> const &m, N c) {
    matrix4<T> new_mat(m.row1 / c, m.row2 / c, m.row3 / c, m.row4 / c);
    return new_mat;
}

template<std::floating_point T, typename N>
constexpr matrix2<T> operator/(N c, matrix2<T> const &m) {
    matrix2<T> new_mat(m.row1 / c, m.row2 / c);
    return new_mat;
}

template<std::floating_point T, typename N>
constexpr matrix3<T> operator/(N c, matrix3<T> const &m) {
    matrix3<T> new_mat(m.row1 / c, m.row2 / c, m.row3 / c);
    return new_mat;
}

template<std::floating_point T, typename N>
constexpr matrix4<T> operator/(N c, matrix4<T> const &m) {
    matrix4<T> new_mat(m.row1 / c, m.row2 / c, m.row3 / c, m.row4 / c);
    return new_mat;
}

/* Boolean Operators */
template<std::floating_point T>
constexpr bool operator==(matrix2<T> const &m, matrix2<T> const &m2) {
    if(m.row1 == m2.row1 && m.row2 == m2.row2) return true;
    return false;
}

template<std::floating_point T>
constexpr bool operator==(matrix3<T> const &m, matrix3<T> const &m2) {
    if(m.row1 == m2.row1 && m.row2 == m2.row2 && m.row3 == m2.row3) return true;
    return false;
}

template<std::floating_point T>
constexpr bool operator==(matrix4<T> const &m, matrix4<T> const &m2) {
    if(m.row1 == m2.row1 && m.row2 == m2.row2 && m.row3 == m2.row3 && m.row4 == m2.row4) return true;
    return false;
}

template<std::floating_point T>
constexpr bool operator!=(matrix2<T> const &m, matrix2<T> const &m2) {
    return !(m == m2);
}

template<std::floating_point T>
constexpr bool operator!=(matrix3<T> const &m, matrix3<T> const &m2) {
    return !(m == m2);
}

template<std::floating_point T>
constexpr bool operator!=(matrix4<T> const &m, matrix4<T> const &m2) {
    return !(m == m2);
}

/* Determinant */
template<std::floating_point T>
constexpr T determinant(matrix2<T> const &m) {
    return ((m[0][0] * m[1][1]) - (m[0][1] * m[1][0]));
}

template<std::floating_point T>
constexpr T determinant(matrix3<T> const &m) {
    T det1 = m[0][0] * ((m[1][1] * m[2][2]) - (m[1][2] * m[2][1]));
    T det2 = m[0][1] * ((m[1][0] * m[2][2]) - (m[1][2] * m[2][0]));
    T det3 = m[0][2] * ((m[1][0] * m[2][1]) - (m[1][1] * m[2][0]));

    return det1 - det2 + det3;
}

template<std::floating_point T>
constexpr T determinant(matrix4<T> const &m) {
    matrix3<T> mat1(vector3<T>(m[1][1], m[1][2], m[1][3]), vector3<T>(m[2][1], m[2][2], m[2][3]), vector3<T>(m[3][1], m[3][2], m[3][3]));
    matrix3<T> mat2(vector3<T>(m[1][0], m[1][2], m[1][3]), vector3<T>(m[2][0], m[2][2], m[2][3]), vector3<T>(m[3][0], m[3][2], m[3][3]));
    matrix3<T> mat3(vector3<T>(m[1][0], m[1][1], m[1][3]), vector3<T>(m[2][0], m[2][1], m[2][3]), vector3<T>(m[3][0], m[3][1], m[3][3]));
    matrix3<T> mat4(vector3<T>(m[1][0], m[1][1], m[1][2]), vector3<T>(m[2][0], m[2][1], m[2][2]), vector3<T>(m[3][0], m[3][1], m[3][2]));

    T det1 = m[0][0] * determinant(mat1);
    T det2 = m[0][1] * determinant(mat2);
    T det3 = m[0][2] * determinant(mat3);
    T det4 = m[0][3] * determinant(mat4);

    return det1 - det2 + det3 - det4;
}

/* Inverse */
template<std::floating_point T>
constexpr matrix2<T> inverse(matrix2<T> const &m) {
    matrix2<T> new_mat(vector2<T>(m[1][1], -1*m[0][1]), vector2<T>(-1*m[1][0], m[0][0]));
    new_mat *= 1/determinant(new_mat);
    return new_mat;
}

template<std::floating_point T>
constexpr matrix3<T> inverse(matrix3<T> const &m) {
    matrix3<T> min = minors_matrix(m);
    matrix3<T> cof = cofactors_matrix(min);
    matrix3<T> transpose = transpose_matrix(cof);
    matrix3<T> inverse = (1 / determinant(m)) * transpose; //TODO: Negative 0
    return inverse;
}

template<std::floating_point T>
constexpr matrix4<T> inverse(matrix4<T> const &m) {
    matrix4<T> min = minors_matrix(m);
    matrix4<T> cof = cofactors_matrix(min);
    matrix4<T> transpose = transpose_matrix(cof);
    matrix4<T> inverse = (1 / determinant(m)) * transpose;
    return inverse;
}

/* Printing */
template<std::floating_point T>
constexpr void print_mat(matrix2<T> const &m) {
    int i = 0;
    while(i < 2) {
        std::cout << ggl::to_string(m[i]) << "\n";
        i++;
    }
}

template<std::floating_point T>
constexpr void print_mat(matrix3<T> const &m) {
    int i = 0;
    while(i < 3) {
        std::cout << ggl::to_string(m[i]) << "\n";
        i++;
    }
}

template<std::floating_point T>
constexpr void print_mat(matrix4<T> const &m) {
    int i = 0;
    while(i < 4) {
	    std::cout << ggl::to_string(m[i]) << "\n";
        i++;
    }
}

/* Special Matrices*/

template<std::floating_point T>
constexpr ggl::matrix3<T> minors_matrix(matrix3<T> const &m) {
    matrix3<T> new_mat;

    new_mat[0][0] = determinant(matrix2<T>(vector2<T>(m[1][1], m[1][2]), vector2<T>(m[2][1], m[2][2])));
    new_mat[0][1] = determinant(matrix2<T>(vector2<T>(m[1][0], m[1][2]), vector2<T>(m[2][0], m[2][2])));
    new_mat[0][2] = determinant(matrix2<T>(vector2<T>(m[1][0], m[1][1]), vector2<T>(m[2][0], m[2][1])));

    new_mat[1][0] = determinant(matrix2<T>(vector2<T>(m[0][1], m[0][2]), vector2<T>(m[2][1], m[2][2])));
    new_mat[1][1] = determinant(matrix2<T>(vector2<T>(m[0][0], m[0][2]), vector2<T>(m[2][0], m[2][2])));
    new_mat[1][2] = determinant(matrix2<T>(vector2<T>(m[0][0], m[0][1]), vector2<T>(m[2][0], m[2][1])));

    new_mat[2][0] = determinant(matrix2<T>(vector2<T>(m[0][1], m[0][2]), vector2<T>(m[1][1], m[1][2])));
    new_mat[2][1] = determinant(matrix2<T>(vector2<T>(m[0][0], m[0][2]), vector2<T>(m[1][0], m[1][2])));
    new_mat[2][2] = determinant(matrix2<T>(vector2<T>(m[0][0], m[0][1]), vector2<T>(m[1][0], m[1][1])));

    return new_mat;
}

template<std::floating_point T>
constexpr ggl::matrix4<T> minors_matrix(matrix4<T> const & m) {
    matrix4<T> new_mat;

    new_mat[0][0] = determinant(matrix3<T>(vector3<T>(m[1][1], m[1][2], m[1][3]), vector3<T>(m[2][1], m[2][2], m[2][3]), vector3<T>(m[3][1], m[3][2], m[3][3])));
    new_mat[0][1] = determinant(matrix3<T>(vector3<T>(m[1][0], m[1][2], m[1][3]), vector3<T>(m[2][0], m[2][2], m[2][3]), vector3<T>(m[3][0], m[3][2], m[3][3])));
    new_mat[0][2] = determinant(matrix3<T>(vector3<T>(m[1][0], m[1][1], m[1][3]), vector3<T>(m[2][0], m[2][1], m[2][3]), vector3<T>(m[3][0], m[3][1], m[3][3])));
    new_mat[0][3] = determinant(matrix3<T>(vector3<T>(m[1][0], m[1][1], m[1][2]), vector3<T>(m[2][0], m[2][1], m[2][2]), vector3<T>(m[3][0], m[3][1], m[3][2])));

    new_mat[1][0] = determinant(matrix3<T>(vector3<T>(m[0][1], m[0][2], m[0][3]), vector3<T>(m[2][1], m[2][2], m[2][3]), vector3<T>(m[3][1], m[3][2], m[3][3])));
    new_mat[1][1] = determinant(matrix3<T>(vector3<T>(m[0][0], m[0][2], m[0][3]), vector3<T>(m[2][0], m[2][2], m[2][3]), vector3<T>(m[3][0], m[3][2], m[3][3])));
    new_mat[1][2] = determinant(matrix3<T>(vector3<T>(m[0][0], m[0][1], m[0][3]), vector3<T>(m[2][0], m[2][1], m[2][3]), vector3<T>(m[3][0], m[3][1], m[3][3])));
    new_mat[1][3] = determinant(matrix3<T>(vector3<T>(m[0][0], m[0][1], m[0][2]), vector3<T>(m[2][0], m[2][1], m[2][2]), vector3<T>(m[3][0], m[3][1], m[3][2])));

    new_mat[2][0] = determinant(matrix3<T>(vector3<T>(m[0][1], m[0][2], m[0][3]), vector3<T>(m[1][1], m[1][2], m[1][3]), vector3<T>(m[3][1], m[3][2], m[3][3])));
    new_mat[2][1] = determinant(matrix3<T>(vector3<T>(m[0][0], m[0][2], m[0][3]), vector3<T>(m[1][0], m[1][2], m[1][3]), vector3<T>(m[3][0], m[3][2], m[3][3])));
    new_mat[2][2] = determinant(matrix3<T>(vector3<T>(m[0][0], m[0][1], m[0][3]), vector3<T>(m[1][0], m[1][1], m[1][3]), vector3<T>(m[3][0], m[3][1], m[3][3])));
    new_mat[2][3] = determinant(matrix3<T>(vector3<T>(m[0][0], m[0][1], m[0][2]), vector3<T>(m[1][0], m[1][1], m[1][2]), vector3<T>(m[3][0], m[3][1], m[3][2])));

    new_mat[3][0] = determinant(matrix3<T>(vector3<T>(m[0][1], m[0][2], m[0][3]), vector3<T>(m[1][1], m[1][2], m[1][3]), vector3<T>(m[2][1], m[2][2], m[2][3])));
    new_mat[3][1] = determinant(matrix3<T>(vector3<T>(m[0][0], m[0][2], m[0][3]), vector3<T>(m[1][0], m[1][2], m[1][3]), vector3<T>(m[2][0], m[2][2], m[2][3])));
    new_mat[3][2] = determinant(matrix3<T>(vector3<T>(m[0][0], m[0][1], m[0][3]), vector3<T>(m[1][0], m[1][1], m[1][3]), vector3<T>(m[2][0], m[2][1], m[2][3])));
    new_mat[3][3] = determinant(matrix3<T>(vector3<T>(m[0][0], m[0][1], m[0][2]), vector3<T>(m[1][0], m[1][1], m[1][2]), vector3<T>(m[2][0], m[2][1], m[2][2])));

    return new_mat;
}

template<std::floating_point T>
constexpr ggl::matrix3<T> cofactors_matrix(matrix3<T> const &m) {
    matrix3<T> new_mat = m;
    new_mat[0][1] *= -1;
    new_mat[1][0] *= -1;
    new_mat[1][2] *= -1;
    new_mat[2][1] *= -1;
    return new_mat;
}

template<std::floating_point T>
constexpr ggl::matrix4<T> cofactors_matrix(matrix4<T> const &m) {
    matrix4<T> new_mat = m;
    new_mat[0][1] *= -1;
    new_mat[0][3] *= -1;
    new_mat[1][0] *= -1;
    new_mat[1][2] *= -1;
    new_mat[2][1] *= -1;
    new_mat[2][3] *= -1;
    new_mat[3][0] *= -1;
    new_mat[3][2] *= -1;
    return new_mat;
}

template<std::floating_point T>
constexpr ggl::matrix3<T> transpose_matrix(matrix3<T> const &m) {
    matrix3<T> new_mat = m;
    new_mat[0][1] = m[1][0];
    new_mat[1][0] = m[0][1];
    new_mat[0][2] = m[2][0];
    new_mat[2][0] = m[0][2];
    new_mat[1][2] = m[2][1];
    new_mat[2][1] = m[1][2];
    return new_mat;
}

template<std::floating_point T>
constexpr ggl::matrix4<T> transpose_matrix(matrix4<T> const &m) {
    matrix4<T> new_mat = m;
    new_mat[0][1] = m[1][0];
    new_mat[1][0] = m[0][1];
    new_mat[0][2] = m[2][0];
    new_mat[2][0] = m[0][2];
    new_mat[1][2] = m[2][1];
    new_mat[2][1] = m[1][2];
    new_mat[0][3] = m[3][0];
    new_mat[3][0] = m[0][3];
    new_mat[1][3] = m[3][1];
    new_mat[3][1] = m[1][3];
    new_mat[3][2] = m[2][3];
    new_mat[2][3] = m[3][2];
    return new_mat;
}

/* Translation */
template<std::floating_point T>
constexpr matrix3<T> translation_matrix3(vector2<T> const &v) {
    ggl::matrix3<T> new_mat(1);
    new_mat.row1.z = static_cast<T>(v.x);
    new_mat.row2.z = static_cast<T>(v.y);
    return new_mat;
}

template<std::floating_point T>
constexpr matrix3<T> translation_matrix3(vector3<T> const &v) {
    ggl::matrix3<T> new_mat(1);
    new_mat.row1.z = static_cast<T>(v.x);
    new_mat.row2.z = static_cast<T>(v.y);
    new_mat.row3.x = static_cast<T>(v.z);
    new_mat.row3.y = static_cast<T>(v.z);
    return new_mat;
}

template<std::floating_point T>
constexpr matrix4<T> translation_matrix4(vector3<T> const &v) {
    ggl::matrix4<T> new_mat(1);
    new_mat.row1.w = static_cast<T>(v.x);
    new_mat.row2.w = static_cast<T>(v.y);
    new_mat.row3.w = static_cast<T>(v.z);
    return new_mat;
}

template<std::floating_point T>
constexpr matrix4<T> translation_matrix4(vector4<T> const &v) {
    ggl::matrix4<T> new_mat(1);
    new_mat.row1.w = static_cast<T>(v.x);
    new_mat.row2.w = static_cast<T>(v.y);
    new_mat.row3.w = static_cast<T>(v.z);
    new_mat.row4.x = static_cast<T>(v.w);
    new_mat.row4.y = static_cast<T>(v.w);
    new_mat.row4.z = static_cast<T>(v.w);
    return new_mat;
}

template<std::floating_point T>
constexpr matrix3<T> translate(matrix3<T> const &m, vector3<T> const &v) {
    return m * translation_matrix3(v);
}

template<std::floating_point T>
constexpr matrix3<T> translate(matrix3<T> const &m, vector2<T> const &v) {
    return m * translation_matrix3(v);
}

template<std::floating_point T>
constexpr matrix4<T> translate(matrix4<T> const &m, vector4<T> const &v) {
    return m * translation_matrix4(v);
}

template<std::floating_point T>
constexpr matrix4<T> translate(matrix4<T> const &m, vector3<T> const &v) {
    return m * translation_matrix4(v);
}

/* Rotation */
template<std::floating_point T>
constexpr ggl::matrix3<T> rotation_matrix3_x(T angle) {
    ggl::matrix3<T> new_mat(1);
    new_mat.row2.y = std::cos(angle);
    new_mat.row2.z = -1 * std::sin(angle);
    new_mat.row3.y = std::sin(angle);
    new_mat.row3.z = std::cos(angle);
    return new_mat;
}

template<std::floating_point T>
constexpr ggl::matrix3<T> rotation_matrix3_y(T angle) {
    ggl::matrix3<T> new_mat(1);
    new_mat.row1.x = std::cos(angle);
    new_mat.row1.y = std::sin(angle);
    new_mat.row3.x = -1 * std::sin(angle);
    new_mat.row3.y = std::cos(angle);
    return new_mat;
}

template<std::floating_point T>
constexpr ggl::matrix3<T> rotation_matrix3_z(T angle) {
    ggl::matrix3<T> new_mat(1);
    new_mat.row1.x = std::cos(angle);
    new_mat.row1.y = -1 * std::sin(angle);
    new_mat.row2.x = std::sin(angle);
    new_mat.row2.y = std::cos(angle);
    return new_mat;
}

template<std::floating_point T>
constexpr ggl::matrix4<T> rotation_matrix4_x(T angle) {
    ggl::matrix4<T> new_mat(1);
    new_mat.row2.y = std::cos(angle);
    new_mat.row2.z = -1 * std::sin(angle);
    new_mat.row3.y = std::sin(angle);
    new_mat.row3.z = std::cos(angle);
    return new_mat;
}

template<std::floating_point T>
constexpr ggl::matrix4<T> rotation_matrix4_y(T angle) {
    ggl::matrix4<T> new_mat(1);
    new_mat.row1.x = std::cos(angle);
    new_mat.row1.y = std::sin(angle);
    new_mat.row3.x = -1 * std::sin(angle);
    new_mat.row3.y = std::cos(angle);
    return new_mat;
}

template<std::floating_point T>
constexpr ggl::matrix4<T> rotation_matrix4_z(T angle) {
    ggl::matrix4<T> new_mat(1);
    new_mat.row1.x = std::cos(angle);
    new_mat.row1.y = -1 * std::sin(angle);
    new_mat.row2.x = std::sin(angle);
    new_mat.row2.y = std::cos(angle);
    return new_mat;
}

template<std::floating_point T, std::floating_point N>
constexpr vector3<T> rotate_x(vector2<T> const &v, N angle) {
    angle = static_cast<T>(angle);
    vector3<T> new_vec(v.x, v.y, 0);
    new_vec = rotation_matrix3_x(angle) * new_vec;
    return new_vec;
}

template<std::floating_point T, std::floating_point N>
constexpr vector3<T> rotate_y(vector2<T> const &v, N angle) {
    angle = static_cast<T>(angle);
    vector3<T> new_vec(v.x, v.y, 0);
    new_vec = rotation_matrix3_y(angle) * new_vec;
    return new_vec;
}

template<std::floating_point T, std::floating_point N>
constexpr vector3<T> rotate_z(vector2<T> const &v, N angle) {
    angle = static_cast<T>(angle);
    vector3<T> new_vec(v.x, v.y, 0);
    new_vec = rotation_matrix3_z(angle) * new_vec;
    return new_vec;
}

template<std::floating_point T, std::floating_point N>
constexpr vector3<T> rotate_x(vector3<T> const &v, N angle) {
    angle = static_cast<T>(angle);
    vector3<T> new_vec;
    new_vec = rotation_matrix3_x(angle) * v;
    return new_vec;
}

template<std::floating_point T, std::floating_point N>
constexpr vector3<T> rotate_y(vector3<T> const &v, N angle) {
    angle = static_cast<T>(angle);
    vector3<T> new_vec;
    new_vec = rotation_matrix3_y(angle) * v;
    return new_vec;
}

template<std::floating_point T, std::floating_point N>
constexpr vector3<T> rotate_z(vector3<T> const &v, N angle) {
    angle = static_cast<T>(angle);
    vector3<T> new_vec;
    new_vec = rotation_matrix3_z(angle) * v;
    return new_vec;
}

template<std::floating_point T, std::floating_point N>
constexpr vector4<T> rotate_x(vector4<T> const &v, N angle) {
    vector4<T> new_vec;
    vector3<T> temp_vec(v.x, v.y, v.z);

    new_vec = rotate_x(temp_vec, angle);
    new_vec.w = v.w;
    return new_vec;
}

template<std::floating_point T, std::floating_point N>
constexpr vector4<T> rotate_y(vector4<T> const &v, N angle) {
    vector4<T> new_vec;
    vector3<T> temp_vec(v.x, v.y, v.z);

    new_vec = rotate_y(temp_vec, angle);
    new_vec.w = v.w;
    return new_vec;
}

template<std::floating_point T, std::floating_point N>
constexpr vector4<T> rotate_z(vector4<T> const &v, N angle) {
    vector4<T> new_vec;
    vector3<T> temp_vec(v.x, v.y, v.z);

    new_vec = rotate_z(temp_vec, angle);
    new_vec.w = v.w;
    return new_vec;
}

template<std::floating_point T, std::floating_point N>
constexpr matrix3<T> rotate_x(matrix3<T> const &m, N angle) {
    return m * rotation_matrix4_x(angle);
}

template<std::floating_point T, std::floating_point N>
constexpr matrix3<T> rotate_y(matrix3<T> const &m, N angle) {
    return m * rotation_matrix3_y(angle);
}

template<std::floating_point T, std::floating_point N>
constexpr matrix3<T> rotate_z(matrix3<T> const &m, N angle) {
    return m * rotation_matrix3_z(angle);
}

template<std::floating_point T, std::floating_point N>
constexpr matrix4<T> rotate_x(matrix4<T> const &m, N angle) {
    return m * rotation_matrix4_x(angle);
}

template<std::floating_point T, std::floating_point N>
constexpr matrix4<T> rotate_y(matrix4<T> const &m, N angle) {
    return m * rotation_matrix4_y(angle);
}

template<std::floating_point T, std::floating_point N>
constexpr matrix4<T> rotate_z(matrix4<T> const &m, N angle) {
    return m * rotation_matrix4_z(angle);
}

/* Scaling */
template<std::floating_point T>
constexpr ggl::matrix3<T> scaling_matrix(T width, T height) {
    ggl::matrix3<T> new_mat(1);
    new_mat.row1.x = width;
    new_mat.row2.y = height;
    return new_mat;
}

template<std::floating_point T>
constexpr ggl::matrix4<T> scaling_matrix(T width, T height, T depth) {
    ggl::matrix4<T> new_mat(1);
    new_mat.row1.x = width;
    new_mat.row2.y = height;
    new_mat.row3.z = depth;
    return new_mat;
}

template<std::floating_point T>
constexpr vector3<T> scale(vector3<T> const &v, vector2<T> const &d) {
    return scaling_matrix(d.x, d.y) * v;
}

template<std::floating_point T>
constexpr vector4<T> scale(vector4<T> const &v, vector3<T> const &d) {
    return scaling_matrix(d.x, d.y, d.z) * v;
}

template<std::floating_point T>
constexpr matrix3<T> scale(matrix3<T> const &m, vector2<T> const &d) {
    matrix3<T> new_mat(1);
    new_mat.row1 = scale(m.row1, d);
    new_mat.row2 = scale(m.row2, d);
    return new_mat;
}

template<std::floating_point T>
constexpr matrix4<T> scale(matrix4<T> const &m, vector3<T> const &d) {
    matrix4<T> new_mat(1);
    new_mat.row1 = scale(m.row1, d);
    new_mat.row2 = scale(m.row2, d);
    new_mat.row3 = scale(m.row3, d);
    return new_mat;
}


/* Pointers */
template<typename T>
constexpr T const* pointer(matrix2<T> const &m) {
    return &m[0][0];
}

template<typename T>
constexpr T* pointer(matrix2<T> &m) {
    return &m[0][0];
}

template<typename T>
constexpr T const* pointer(matrix3<T> const &m) {
    return &m[0][0];
}

template<typename T>
constexpr T* pointer(matrix3<T> &m) {
    return &m[0][0];
}

template<typename T>
constexpr T const* pointer(matrix4<T> const &m) {
    return &m[0][0];
}

template<typename T>
constexpr T* pointer(matrix4<T> &m) {
    return &m[0][0];
}


template<std::floating_point T>
constexpr matrix4<T> get_projection_matrix(T fov, T aspect_ratio, T near, T far)
{
    matrix4<T> proj(0);
    proj[0][0] = (1 / tanf(fov/2)) / aspect_ratio;
    proj[1][1] = (1 / tanf(fov/2));
    proj[2][2] = -((far + near) / (far - near));
    proj[2][3] = (-2 * far * near) / (far - near);    
    proj[3][2] = -1;
    return proj;
}

template<std::floating_point T>
constexpr matrix4<T> get_view_matrix(vector3<T> camera_position, vector3<T> target_position, vector3<T> upVec)
{
    vector3<T> forward = normalize(target_position - camera_position);
    vector3<T> right = normalize(cross(forward, upVec));
    vector3<T> up = cross(right, forward);
   
    matrix4<T> view(1.0f);
    view[0][0] = right.x;
    view[1][0] = right.y;
    view[2][0] = right.z;
    view[0][1] = up.x;
    view[1][1] = up.y;
    view[2][1] = up.z;
    view[0][2] = -forward.x;
    view[1][2] = -forward.y;
    view[2][2] = -forward.z;
    view[0][3] = -dot(right, camera_position);
    view[1][3] = -dot(up, camera_position);
    view[2][3] = dot(forward, camera_position);

    return view;
}

}//namespace ggl

#endif //MATRIX_H