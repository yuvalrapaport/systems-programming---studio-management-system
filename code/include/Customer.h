#ifndef CUSTOMER_H_
#define CUSTOMER_H_
#include <iostream>
#include <vector>
#include <string>
#include "Workout.h"
#include <algorithm>
using namespace std;


bool comp (const Workout* A, const Workout* B); //added

class Customer{
public:
    Customer(std::string c_name, int c_id);
    Customer(std::string c_name, int c_id, int c_worth); //added constructor
    virtual ~Customer();//destructor
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    int getWorth();
private:
    const std::string name;
    const int id;

protected:
    int worth; //added
};


class SweatyCustomer : public Customer {
public:
	SweatyCustomer(std::string name, int id);
    SweatyCustomer(std::string name, int id, int worth);
    virtual ~SweatyCustomer(); //destructor
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
    CheapCustomer(std::string name, int id, int worth);
    virtual ~CheapCustomer(); //destructor
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};


class HeavyMuscleCustomer : public Customer {
public:
	HeavyMuscleCustomer(std::string name, int id);
    HeavyMuscleCustomer(std::string name, int id, int worth);
    virtual ~HeavyMuscleCustomer(); //destructor
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};


class FullBodyCustomer : public Customer {
public:
	FullBodyCustomer(std::string name, int id);
    FullBodyCustomer(std::string name, int id, int worth);
    virtual ~FullBodyCustomer(); //destructor
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
private:
};


#endif