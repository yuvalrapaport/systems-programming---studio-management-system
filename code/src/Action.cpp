#include "Action.h"
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
extern Studio* backup;

BaseAction::BaseAction():errorMsg(""),status(){} //constructor
BaseAction::BaseAction(const BaseAction& other):errorMsg(other.errorMsg),status(other.status){}
BaseAction::~BaseAction(){}
ActionStatus BaseAction::getStatus() const{
    return status;
}
void BaseAction::complete(){
    status = COMPLETED;
}
void BaseAction::error(std::string errorMsg){
    status = ERROR;
    this->errorMsg = errorMsg;
}
std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}

//OpenTrainer
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):trainerId(id),customers(customersList),customersStr(""){}
OpenTrainer::OpenTrainer(const OpenTrainer& other):BaseAction(other),trainerId(other.trainerId), customers(),customersStr(other.customersStr){ //copy constructor
    for (Customer* c : other.customers){
        customers.push_back(c);
    }
}
OpenTrainer::~OpenTrainer(){}

void OpenTrainer::act(Studio &studio){
    for (Customer* c : customers) {
        if (studio.getTrainer(trainerId) != nullptr && !((studio.getTrainer(trainerId))->isOpen())) {
            ((studio.getTrainer(trainerId))->getCustomers()).push_back(c);
        }
        customersStr += c->getName() + "," + c->toString() + " ";
    }

    if (studio.getTrainer(trainerId) == nullptr || (studio.getTrainer(trainerId))->isOpen()) {
        error("Trainer does not exist or is already open");
        cout << getErrorMsg() << endl;
    }
    else {
        (studio.getTrainer(trainerId))->openTrainer();
        complete();
    }
}
std::string OpenTrainer::toString() const{
    string str = "open " + to_string(trainerId) + " " + customersStr;
    if (getStatus() == COMPLETED)
        str+= "Completed";
    else
        str+= "Error: " + getErrorMsg();

    return str;
}

string OpenTrainer::getType(){
    return "open";
}

OpenTrainer* OpenTrainer::copy(){
    return new OpenTrainer(*this);
}

//Order
Order::Order(int id):trainerId(id){}
Order::Order(const Order& other):BaseAction(other),trainerId(other.trainerId){} //copy constructor
Order::~Order(){}
void Order::act(Studio &studio){
    Trainer* trainer = studio.getTrainer(trainerId);
    if (trainer==nullptr || !(trainer->isOpen())) {
        error("trainer does not exist or is not open");
        cout << getErrorMsg() << endl;
    }
    else{
        vector<Customer*> customersList = trainer->getCustomers();
        if (!trainer->wasOrdered()) {
            trainer->resetOrder();
            for (Customer *c: customersList) {
                vector<int> workoutId = c->order(studio.getWorkoutOptions());
                trainer->setSalary(c->getWorth());
                for (int w: workoutId) {
                    (trainer->getOrders()).push_back(make_pair(c->getId(), (studio.getWorkoutOptions())[w]));
                   // cout << c->getName() << " Is Doing " << (studio.getWorkoutOptions())[w].getName() << endl;
                }
            }
        }
            for (OrderPair op : trainer->getOrders()) {
                string wName;
                wName = (op.second).getName();
                string cName = (trainer->getCustomer(op.first))->getName();
                cout << cName << " Is Doing " << wName <<endl;
            }
            complete();
        }


}

std::string Order::toString() const{
    string str = "order " + to_string(trainerId) + " ";

    if (getStatus() == COMPLETED)
        str+= "Completed";
    else
        str+= "Error: " + getErrorMsg();

    return str;
}

string Order::getType(){
    return "order";
}

Order* Order::copy(){
    return new Order(*this);
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId):srcTrainer(src),dstTrainer(dst),id(customerId){}
MoveCustomer::MoveCustomer(const MoveCustomer& other):BaseAction(other),srcTrainer(other.srcTrainer), dstTrainer(other.dstTrainer), id(other.id){} //copy constructor
MoveCustomer::~MoveCustomer(){}
void MoveCustomer::act(Studio &studio){
    Trainer* srcT = studio.getTrainer(srcTrainer);
    Trainer* dstT = studio.getTrainer(dstTrainer);
    bool errorReason1 = srcT==nullptr || dstT==nullptr || !srcT->isOpen() || !dstT->isOpen();
    bool errorReason2 = (srcT->getCustomer(id)) == nullptr;
    bool errorReason3 = dstT->getCapacity() == dstT->getNumOfCus();
    if (errorReason1 || errorReason2 || errorReason3){
        error("Cannot move customer");
        cout << getErrorMsg() << endl;
    }
    else {
        Customer *customer = srcT->getCustomer(id);
        (dstT->getCustomers()).push_back(customer);
        int index = 0;
        for (Customer *c: srcT->getCustomers()) { //moving pointer to customer from src trainer to dst trainer
            if (c->getId() == id) {
                (srcT->getCustomers()).erase((srcT->getCustomers()).begin() + index);
                break;
            }
            index++;
        }


        vector<OrderPair> copy;
        for (OrderPair op: srcT->getOrders()) { //moving all customer's orders from src trainer to dst trainer
            if (op.first != id) {
                copy.push_back(make_pair(op.first,op.second));
            }
            else
                (dstT->getOrders()).push_back(make_pair(op.first, op.second));
        }
        (srcT->getOrders()).swap(copy);
        srcT->setNumOfCus(-1);
        dstT->setNumOfCus(1);
        srcT->setSalary(-(customer->getWorth()));
        dstT->setSalary(customer->getWorth());
        if (srcT->getCustomers().empty()) {
            Close *cl = new Close(srcTrainer);
            cl->act(studio);
            delete cl;
        }
        complete();
    }
}

std::string MoveCustomer::toString() const{
    string str = "move " + to_string(srcTrainer) + " " + to_string(dstTrainer) + " " + to_string(id)+ " ";

    if (getStatus() == COMPLETED)
        str+= "Completed";
    else
        str+= "Error: " + getErrorMsg();

    return str;
}

string MoveCustomer::getType(){
    return "move";
}

MoveCustomer* MoveCustomer::copy(){
    return new MoveCustomer(*this);
}

//Close
Close::Close(int id):trainerId(id){}
Close::Close(const Close& other):BaseAction(other),trainerId(other.trainerId){} //copy constructor
Close::~Close(){}
void Close::act(Studio &studio){
    Trainer* trainer = studio.getTrainer(trainerId);
    if(trainer == nullptr || !trainer->isOpen()){
        error("Trainer does not exist or is not open");
        cout << getErrorMsg() << endl;
    }

    else if (trainer->wasOpened()){
        trainer->closeTrainer();
        trainer->resetOrder();
        trainer->setNumOfCus(-(trainer->getNumOfCus()));
        trainer->clear();
        complete();
        cout << "Trainer " << trainerId << " closed. Salary " << trainer->getSalary() << "NIS" << endl;
    }
}


std::string Close::toString() const{
    string str = "close " + to_string(trainerId) + " ";

    if (getStatus() == COMPLETED)
        str+= "Completed";
    else
        str+= "Error: " + getErrorMsg();

    return str;
}

string Close::getType(){
    return "close";
}

Close* Close::copy(){
    return new Close(*this);
}


//CloseAll
CloseAll::CloseAll(){}
CloseAll::~CloseAll(){}
void CloseAll::act(Studio &studio){
    studio.closeStudio();
    for (int i = 0; i<studio.getNumOfTrainers(); i++){
        Trainer* trainer = studio.getTrainer(i);
        Close* cl = new Close(i);
        if (trainer->isOpen()){
            cl->act(studio);
        }
        delete cl;
    }
    complete();
}
std::string CloseAll::toString() const{
    return "closeall Completed";
}

string CloseAll::getType(){
    return "closeall";
}

CloseAll* CloseAll::copy(){
    return new CloseAll();
}

//PrintWorkoutOptions
PrintWorkoutOptions::PrintWorkoutOptions(){}
PrintWorkoutOptions::~PrintWorkoutOptions(){}
void PrintWorkoutOptions::act(Studio &studio){
    vector <Workout> workouts = studio.getWorkoutOptions();
    for (Workout w : workouts){
        string type;
        WorkoutType Wtype = w.getType();
        if (Wtype == ANAEROBIC)
            type = "Anaerobic";
        if (Wtype == MIXED)
            type = "Mixed";
        if (Wtype == CARDIO)
            type = "Cardio";
        cout << w.getName() << ", " << type << ", " << w.getPrice() << endl;
    }
    complete();
}
std::string PrintWorkoutOptions::toString() const{
    return "workout_options Completed";
}

string PrintWorkoutOptions::getType(){
    return "workout_options";
}

PrintWorkoutOptions* PrintWorkoutOptions::copy(){
    return new PrintWorkoutOptions();
}

//PrintTrainerStatus
PrintTrainerStatus::PrintTrainerStatus(int id):trainerId(id){}
PrintTrainerStatus::PrintTrainerStatus(const PrintTrainerStatus& other):BaseAction(other), trainerId(other.trainerId){} //copy constructor
PrintTrainerStatus::~PrintTrainerStatus(){}
void PrintTrainerStatus::act(Studio &studio){
    Trainer* trainer = studio.getTrainer(trainerId);
    if (!trainer->isOpen())
        cout << "Trainer " << trainerId << " status: closed" << endl;
    else{
        cout << "Trainer " << trainerId << " status: open" << endl;
        cout << "Customers:" << endl;
        for (Customer* c : trainer->getCustomers())
            cout << c->getId() << " " << c->getName() << endl;

        cout << "Orders:" << endl;
        for (OrderPair op : trainer->getOrders()){
            string wName;
            int wPrice;
            wName = (op.second).getName();
            wPrice = (op.second).getPrice();
            cout << wName << " " << wPrice << "NIS " << op.first << endl;
        }
        cout << "Current Trainer's Salary: " << trainer->getSalary() << "NIS" << endl;
        complete();
    }

}
std::string PrintTrainerStatus::toString() const{
    return "status " + to_string(trainerId) + " Completed";
}

string PrintTrainerStatus::getType(){
    return "status";
}

PrintTrainerStatus* PrintTrainerStatus::copy(){
    return new PrintTrainerStatus(*this);
}

//PrintActionsLog
PrintActionsLog::PrintActionsLog(){}
PrintActionsLog::~PrintActionsLog(){}
void PrintActionsLog::act(Studio &studio){
    for (BaseAction* ba : studio.getActionsLog()){
        cout << ba->toString() << endl;
    }
    studio.addAction(this);
}
std::string PrintActionsLog::toString() const{
    return  "log Completed";
}

string PrintActionsLog::getType(){
    return "log";
}

PrintActionsLog* PrintActionsLog::copy(){
    return new PrintActionsLog();
}

//BackupStudio
BackupStudio::BackupStudio(){}
BackupStudio::~BackupStudio(){}
void BackupStudio::act(Studio &studio){
    if (backup!=nullptr) {
        *backup = studio;
    }
    else {
        backup = new Studio(studio);
    }
    complete();
}
std::string BackupStudio::toString() const{
    return "backup Completed";
}

string BackupStudio::getType(){
    return "backup";
}

BackupStudio* BackupStudio::copy(){
    return new BackupStudio();
}
//RestoreStudio
RestoreStudio::RestoreStudio(){}
RestoreStudio::RestoreStudio(const RestoreStudio& other):BaseAction(other){}
RestoreStudio::~RestoreStudio(){}
void RestoreStudio::act(Studio &studio){
    if (backup != nullptr) {
        complete();
        studio = *backup;
    }
    else {
        error("No backup available");
        cout << getErrorMsg() << endl;
    }
}
std::string RestoreStudio::toString() const{
    string str = "restore ";
    if (getStatus() == COMPLETED)
        str+= "Completed";
    else
        str+= "Error: " + getErrorMsg();
    return str;
}

string RestoreStudio::getType(){
    return "restore";
}

RestoreStudio* RestoreStudio::copy(){
    return new RestoreStudio(*this);
}
