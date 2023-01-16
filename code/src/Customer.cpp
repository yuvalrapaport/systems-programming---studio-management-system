#include "Customer.h"
#include <iostream>
using namespace std;

//Customer
Customer::Customer(std::string c_name, int c_id):name(c_name), id(c_id),worth(0){} //constructor
Customer::Customer(std::string c_name, int c_id, int c_worth):name(c_name), id(c_id),worth(c_worth){}//added constructor
Customer::~Customer(){} //destructor

bool comp (const Workout* A, const Workout* B){
    int priceA = A->getPrice();
    int priceB = B->getPrice();
    return priceA<priceB;
}


bool comp2 (const Workout* A, const Workout* B){
    int priceA = A->getPrice();
    int priceB = B->getPrice();
    return priceA>priceB;
}

std::string Customer::getName() const{
    return name;
}
int Customer::getId() const{
    return id;
}

int Customer:: getWorth(){
    return worth;
}

//SweatyCustomer
SweatyCustomer::SweatyCustomer(std::string name, int id):Customer(name,id){} //use base class constructor
SweatyCustomer::SweatyCustomer(std::string name, int id,int worth):Customer(name,id,worth){}
SweatyCustomer::~SweatyCustomer(){ //destructor
}
std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){
    vector <int> plan;
    for (Workout w : workout_options){
        if (w.getType() == CARDIO){
            int w_id = w.getId();
            plan.push_back(w_id);
            worth += w.getPrice();
        }
    }
    return plan;
}
std::string SweatyCustomer::toString() const{
    return "swt";
}

//CheapCustomer
CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id){} //use base class constructor
CheapCustomer::CheapCustomer(std::string name, int id, int worth):Customer(name,id,worth){}
CheapCustomer::~CheapCustomer(){ //destructor
}
vector<int> CheapCustomer:: order(const vector<Workout> &workout_options) {
    vector <int> plan;
    const Workout* cheapest = &workout_options[0];
    int i =0;
    for (Workout w : workout_options){
        if (w.getPrice()<cheapest->getPrice())
            cheapest = &workout_options[i];

        i++;
    }
    plan.push_back(cheapest->getId());
    worth += cheapest->getPrice();
    return plan;
}
string CheapCustomer:: toString() const{
    return "chp";
}

//HeavyMuscleCustomer
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id):Customer(name,id){} //use base class constructor
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id,int worth):Customer(name,id,worth){}
HeavyMuscleCustomer::~HeavyMuscleCustomer(){ //destructor
}
vector<int> HeavyMuscleCustomer::  order(const vector<Workout> &workout_options){
    vector <int> plan;
    vector <const Workout*> copy;
    int i = 0;
    for (Workout w : workout_options){
      copy.push_back(&workout_options[i]);
      i++;
    }

    stable_sort(copy.begin(), copy.end(), comp2);

    for (const Workout* w : copy){
        if (w->getType() == ANAEROBIC){
            plan.push_back(w->getId());
            worth += w->getPrice();
        }
    }
    return plan;
}
string HeavyMuscleCustomer:: toString() const{
    return "mcl";
}

//FullBodyCustomer
FullBodyCustomer::FullBodyCustomer(std::string name, int id):Customer(name,id){} //use base class constructor
FullBodyCustomer::FullBodyCustomer(std::string name, int id,int worth):Customer(name,id,worth){}
FullBodyCustomer::~FullBodyCustomer(){ //destructor
}
std::vector<int> FullBodyCustomer:: order(const std::vector<Workout> &workout_options){
    vector <int> plan;
    vector <const Workout*> copy;
    vector <const Workout*> copy2;
    int i = 0;
    for (Workout w : workout_options){
        copy.push_back(&workout_options[i]);
        copy2.push_back(&workout_options[i]);
        i++;
    }
    stable_sort(copy.begin(), copy.end(), comp);
    stable_sort(copy2.begin(), copy2.end(), comp2);

    for (const Workout* w : copy){
        if (w->getType() == CARDIO){
            plan.push_back(w->getId());
            worth += w->getPrice();
            break;
        }
    }
    for (const Workout* w : copy2){
        if (w->getType() == MIXED){
            plan.push_back(w->getId());
            worth += w->getPrice();
            break;
        }
    }
    for (const Workout* w : copy){
        if (w->getType() == ANAEROBIC){
            plan.push_back(w->getId());
            worth += w->getPrice();
            break;
        }
    }

    return plan;
}
string FullBodyCustomer:: toString() const{
    return "fbd";
}