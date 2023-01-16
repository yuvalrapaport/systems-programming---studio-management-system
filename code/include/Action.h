#ifndef ACTION_H_
#define ACTION_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Studio.h"
#include "Customer.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

enum ActionStatus{
    COMPLETED, ERROR
};

//Forward declaration
class Studio;

class BaseAction{
public:
    BaseAction();
    BaseAction(const BaseAction& other); //copy constructor
    virtual ~BaseAction(); //destructor
    ActionStatus getStatus() const;
    virtual void act(Studio& studio)=0;
    virtual std::string toString() const=0;
    virtual string getType() = 0;
    virtual BaseAction* copy() =0;

protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    OpenTrainer(const OpenTrainer& other); //copy constructor
    virtual ~OpenTrainer();
    void act(Studio &studio);
    std::string toString() const;
    string getType();
    OpenTrainer* copy();

private:
	const int trainerId;
	std::vector<Customer *> customers;
    string customersStr;
};


class Order : public BaseAction {
public:
    Order(int id);
    Order(const Order& other); //copy constructor
    virtual ~Order();
    void act(Studio &studio);
    std::string toString() const;
    string getType();
    Order* copy();
private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    MoveCustomer(const MoveCustomer& other); //copy constructor
    virtual ~MoveCustomer();
    void act(Studio &studio);
    std::string toString() const;
    string getType();
    MoveCustomer* copy();
private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    Close(const Close& other); //copy constructor
    virtual ~Close();
    void act(Studio &studio);
    std::string toString() const;
    string getType();
    Close* copy();
private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    virtual ~CloseAll();
    void act(Studio &studio);
    std::string toString() const;
    string getType();
    CloseAll* copy();
private:
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    virtual ~PrintWorkoutOptions();
    void act(Studio &studio);
    std::string toString() const;
    string getType();
    PrintWorkoutOptions* copy();
private:
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    PrintTrainerStatus(const PrintTrainerStatus& other); //copy constructor
    virtual ~PrintTrainerStatus();
    void act(Studio &studio);
    std::string toString() const;
    string getType();
    PrintTrainerStatus* copy();

private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    virtual ~PrintActionsLog();
    void act(Studio &studio);
    std::string toString() const;
    string getType();
    PrintActionsLog* copy();
private:
};


class BackupStudio : public BaseAction {
public:
    BackupStudio();
    virtual ~BackupStudio();
    void act(Studio &studio);
    std::string toString() const;
    string getType();
    BackupStudio* copy();
private:
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio();
    RestoreStudio(const RestoreStudio& other);
    virtual ~RestoreStudio();
    void act(Studio &studio);
    std::string toString() const;
    string getType();
    RestoreStudio* copy();

};


#endif