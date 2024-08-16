#ifndef SPRING_H
#define SPRING_H

#include <concepts>
#include <typeinfo>
#include "particle.hpp"

namespace ggl {

template<std::floating_point T>
struct spring {
    int id;
    int id_next;

    T rest_length;
    T stiffness;

    constexpr spring(){
        this->id = 0;
        this->id_next = 1;
        this->rest_length = 0.0f;
        this->stiffness = 100.0f;
    }
    constexpr spring(spring const& s){
        this->id = s.id;
        this->id_next = s.id_next;
        this->rest_length = s.rest_length;
        this->stiffness = s.stiffness;
    }
    explicit constexpr spring(int i, int j, T rest_length, T stiffness){
        this->id = i;
        this->id_next = j;
        this->rest_length = rest_length;
        this->stiffness = stiffness;
    }

    static inline constexpr const char* type() {
        return typeid(T).name();
    }
};

template<std::floating_point T>
vector3<T> spring_force(spring<T> const &s, particle<T> const &p1, particle<T> const &p2){
    float distance = mag(p2.position - p1.position);
    vector3 force = s.stiffness * (distance - s.rest_length) * normalize(p2.position - p1.position);
    return force;
}

} //namespace ggl

#endif //SPRING_H