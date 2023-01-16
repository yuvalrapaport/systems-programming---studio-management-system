#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);
    Trainer(int t_capacity, int _id, int _salary); //added constructor to new fields id and salary
    Trainer(const Trainer &other); //copy constructor
    Trainer (Trainer &&other); //move constructor
    virtual ~Trainer(); //destructor
    Trainer& operator=(const Trainer &other); //copy assignment operator
    Trainer& operator=(Trainer &&other); //move assignment operator
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();
    void clear(); //added
    void setSalary(int add); //added
    void setNumOfCus (int add); //added
    int getNumOfCus(); //added
    bool wasOpened(); //added
    int getId(); //added
    bool wasOrdered(); //added
    void resetOrder(); //added

private:
    int capacity;
    int id; //added
    int salary; //added
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    int numOfCus; //added
    bool opened; //added
    bool ordered; //added
};


#endif