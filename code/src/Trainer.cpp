#include "Trainer.h"
#include "Customer.h"
#include "Workout.h"
#include <vector>
#include <iostream>
using namespace std;

typedef pair<int, Workout> OrderPair;

//constructors
    Trainer::Trainer(int t_capacity):capacity(t_capacity),id(0),salary(0),open(false),customersList(vector<Customer*>()),orderList(vector<OrderPair>()),numOfCus(0),opened(false),ordered(false){}

    Trainer::Trainer(int t_capacity, int _id, int _salary):capacity(t_capacity), id(_id), salary(_salary), open(false),customersList(vector<Customer*>()),orderList(vector<OrderPair>()),numOfCus(0),opened(false),ordered(false){} //added constructor to new fields id and salary

    Trainer::Trainer(const Trainer &other):capacity(other.capacity), id(other.id),salary(other.salary),open(other.open), customersList(),orderList(),numOfCus(other.numOfCus), opened(other.opened),ordered(other.ordered) { //copy constructor
        for (Customer *c: other.customersList) {
            if (c->toString() == "swt")
                customersList.push_back(new SweatyCustomer(c->getName(), c->getId(), c->getWorth()));
            if (c->toString() == "chp")
                customersList.push_back(new CheapCustomer(c->getName(), c->getId(), c->getWorth()));
            if (c->toString() == "mcl")
                customersList.push_back(new HeavyMuscleCustomer(c->getName(), c->getId(), c->getWorth()));
            if (c->toString() == "fbd")
                customersList.push_back(new FullBodyCustomer(c->getName(), c->getId(), c->getWorth()));
        }

        for (OrderPair op : other.orderList){
            orderList.push_back(make_pair(op.first,op.second));
        }
    }

    Trainer::Trainer (Trainer &&other):capacity(other.capacity), id(other.id),salary(other.salary),open(other.open), customersList(other.customersList),orderList(other.orderList),numOfCus(other.numOfCus),opened(other.opened),ordered(other.ordered)  {} //move constructor

    Trainer:: ~Trainer(){ //destructor
        clear();
    }
    Trainer& Trainer::operator=(const Trainer &other){//copy assignment operator
        capacity = other.capacity;
        id = other.id;
        salary = other.salary;
        open = other.open;
        numOfCus = other.numOfCus;
        opened = other.opened;
        ordered = other.ordered;
        for (OrderPair op : other.orderList){
            orderList.push_back(make_pair(op.first,op.second));
        }
        clear();
        for (Customer* c : other.customersList){    //copy customersList
            if (c->toString() == "swt")
                customersList.push_back(new SweatyCustomer(c->getName(), c->getId(), c->getWorth()));
            if (c->toString() == "chp")
                customersList.push_back(new CheapCustomer(c->getName(), c->getId(), c->getWorth()));
            if (c->toString() == "mcl")
                customersList.push_back(new HeavyMuscleCustomer(c->getName(), c->getId(), c->getWorth()));
            if (c->toString() == "fbd")
                customersList.push_back(new FullBodyCustomer(c->getName(), c->getId(), c->getWorth()));
        }

        return *this;
    }
    Trainer& Trainer::operator=(Trainer &&other){ //move assignment operator
        capacity = other.capacity;
        id = other.id;
        salary = other.salary;
        open = other.open;
        numOfCus=other.numOfCus;
        opened = other.opened;
        ordered = other.ordered;
        clear();
        for (OrderPair op : other.orderList) {
            orderList.push_back(make_pair(op.first, op.second));
        }
        customersList = other.customersList;
        return *this;
    }

    void Trainer:: clear(){
        for(Customer* c : customersList){
            delete c;
        }
        vector<Customer*>().swap(customersList);
        vector<OrderPair>().swap(orderList);
    }

    //functions
    int Trainer::getCapacity() const {
        return capacity;
    }

    void Trainer::addCustomer(Customer* customer){
        customersList.push_back(customer);
    }

    void Trainer::removeCustomer(int id){
        int i =0;
        for (Customer* c : customersList) {
            if (c->getId() == id) {
                delete c;
                customersList.erase(customersList.begin()+i-1);
            }
            i++;
        }
    }
    Customer* Trainer::getCustomer(int id){
        for (Customer* c : customersList){
            if (c->getId() == id)
                return c;
        }
        return nullptr;
    }

    std::vector<Customer*>& Trainer::getCustomers(){
        return customersList;
    }
    std::vector<OrderPair>& Trainer::getOrders(){
        return orderList;
    }
    void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options){}

    void Trainer::openTrainer(){
        open = true;
        opened = true;
    }

    void Trainer::closeTrainer(){
        open = false;
    }

    int Trainer::getSalary(){
        return salary;
    }
    bool Trainer::isOpen(){
        return open;
    }
    void Trainer::setSalary(int add){
        salary += add;
    }
    void Trainer::setNumOfCus(int add){
        numOfCus += add;
    }
    int Trainer::getNumOfCus(){
        return numOfCus;
    }
    bool Trainer::wasOpened(){
        return opened;
    }
    int Trainer::getId(){
        return id;
    }

    bool Trainer::wasOrdered(){
        return ordered;
    }

    void Trainer::resetOrder(){
        ordered = !ordered;
    }