#include <iostream>
using namespace std;
#include "Workout.h"


Workout::Workout(int w_id, string w_name, int w_price, WorkoutType w_type):id(w_id), name(w_name), price(w_price), type(w_type){} //constructor
Workout::Workout (Workout &&other):id(other.id), name(other.name), price(other.price), type(other.type){
}
Workout::Workout (const Workout &other):id(other.id), name(other.name), price(other.price), type(other.type){}

Workout& Workout::operator=(Workout &&other) {
    return *this;
}
Workout& Workout::operator=(const Workout &other){
    return *this;
}

    int Workout::getId() const{
    return id;
}
string Workout::getName() const{
    return name;
}
int Workout::getPrice() const{
    return price;
}
WorkoutType Workout::getType() const{
    return type;
}
