#include "Studio.h"
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <istream>
using namespace std;

    Studio:: Studio():open(false), trainers(), workout_options(), actionsLog(), customersId(0),numOfTrainers(0){}

    Studio:: Studio(const std::string &configFilePath):open(false), trainers(), workout_options(), actionsLog(), customersId(0),numOfTrainers(0){
        std::ifstream file(configFilePath);
        char line[256];
        file.getline(line,256);
        file.getline(line,256);
        int numTrainers = line[0]- '0'; //number of trainers
        numOfTrainers = numTrainers;
        file.getline(line,256);
        file.getline(line,256);
        file.getline(line,256);
        int prevIndex = 0;
        int currIndex = 0;
        for (int i = 0; i<numTrainers; i++) { //initializing field trainers, with capacities
            string str;
            while ( line[currIndex] != '\0' && line[currIndex] != ',') {
                currIndex ++;
            }
            for (int j = prevIndex; j<currIndex; j++) {
                str.push_back(line[j]);
            }

            Trainer* tempTrainer = new Trainer(std::stoi(str), i, 0);
            trainers.push_back(tempTrainer);
            currIndex++;
            prevIndex = currIndex;
        }
        int id =0;
        file.getline(line, 256);
        file.getline(line, 256);
        while (file) {
            string name,type,price;
            WorkoutType t;
            getline(file,name,',') ;
            getline(file,type,' ');
            getline(file,type,',');
            if (type[0] == '\0')
                continue;
            getline(file,price);
            if (type[0]=='A')
                t = ANAEROBIC;
            else if (type[0]=='M')
                t = MIXED;
            else
                t = CARDIO;

            Workout w = Workout(id,name,stoi(price),t);
            workout_options.push_back(w);
            id++;
        }

    }

    Studio::Studio(const Studio &other):open(other.open),trainers(),workout_options(),actionsLog(), customersId(other.customersId),numOfTrainers(other.numOfTrainers) {//copy constructor
        for (Trainer* t : other.trainers){
            trainers.push_back(new Trainer(*t));
        }

        for (BaseAction* ac : other.actionsLog){
            actionsLog.push_back(ac->copy());
        }
        for (Workout w: other.workout_options){
            workout_options.push_back(w);}
    }
    Studio::Studio(Studio &&other):open(other.open),trainers(other.trainers),workout_options(other.workout_options),actionsLog(other.actionsLog), customersId(other.customersId),numOfTrainers(other.numOfTrainers){} //move constructor

    Studio& Studio::operator= (const Studio &other) //copy assignment operator
    {
        open = other.open;
        numOfTrainers = other.numOfTrainers;
        customersId = other.customersId;

        for (Trainer* t: trainers){
            t->clear();
            delete t;
        }
        vector<Trainer*>().swap(trainers);
        for (Trainer* t : other.trainers){
            trainers.push_back(new Trainer(*t));
        }
        vector<Workout>().swap(workout_options);
        for (Workout w : other.workout_options)
            workout_options.push_back(w);

        for (BaseAction* ba : actionsLog) {
                delete ba;
        }

        vector<BaseAction*>().swap(actionsLog);
        for (BaseAction* ac : other.actionsLog){
            actionsLog.push_back(ac->copy());
        }
        return *this;
    }

    Studio& Studio::operator= (Studio &&other) { //move assignment operator
        open = other.open;
        numOfTrainers = other.numOfTrainers;
        trainers = other.trainers;
        workout_options = other.workout_options;
        actionsLog = other.actionsLog;
        customersId = other.customersId;

        return *this;
    }
    Studio::~Studio(){ //destructor
        for (Trainer* t : trainers)
            delete t;
        for (BaseAction* ac : actionsLog)
            delete ac;

    }


    BaseAction* Studio::buildAction(char* command){
        stringstream commandStream(command);
        string com,name,strategy, idTrainer, idDst, idCus;
        int countCus=0;
        vector<Customer*> customersList;
        getline(commandStream, com, ' ');
        if (com == "open"){
            getline(commandStream,idTrainer,' ');
            if (stoi(idTrainer)< getNumOfTrainers()) {
                while (getline(commandStream, name, ',') && ((this->getTrainer(stoi(idTrainer)))->getCapacity()) > countCus && !(((this->getTrainer(stoi(idTrainer)))->isOpen()))) {
                    getline(commandStream, strategy, ' ');
                    if (strategy == "swt")
                        customersList.push_back(new SweatyCustomer(name, customersId, 0));
                    if (strategy == "chp")
                        customersList.push_back(new CheapCustomer(name, customersId, 0));
                    if (strategy == "mcl")
                        customersList.push_back(new HeavyMuscleCustomer(name, customersId, 0));
                    if (strategy == "fbd")
                        customersList.push_back(new FullBodyCustomer(name, customersId, 0));
                    customersId++;
                    countCus++;
                }
                (this->getTrainer(stoi(idTrainer)))->setNumOfCus(countCus);
            }
            OpenTrainer* OT = new OpenTrainer(stoi(idTrainer), customersList);
            actionsLog.push_back(OT);
            return OT;
        }

        if(com == "order"){
            getline(commandStream,idTrainer, ' ');
            Order* ord = new Order(stoi(idTrainer));
            actionsLog.push_back(ord);
            return ord;
        }

        if (com == "move"){
            getline(commandStream,idTrainer,' ');
            getline(commandStream,idDst, ' ');
            getline(commandStream, idCus, ' ');
            MoveCustomer* move = new MoveCustomer(stoi(idTrainer), stoi(idDst), stoi(idCus));
            actionsLog.push_back(move);
            return move;
        }

        if(com == "close"){
            getline(commandStream,idTrainer,' ');
            Close* close = new Close(stoi(idTrainer));
            actionsLog.push_back(close);
            return close;
        }

        if (com == "closeall"){
            CloseAll* closeall = new CloseAll();
            actionsLog.push_back(closeall);
            return closeall;
        }

        if (com == "workout_options"){
            PrintWorkoutOptions* print = new PrintWorkoutOptions();
            actionsLog.push_back(print);
            return print;
        }

        if (com == "status"){
            getline(commandStream,idTrainer,' ');
            PrintTrainerStatus* status = new PrintTrainerStatus(stoi(idTrainer));
            actionsLog.push_back(status);
            return status;
        }

        if (com == "log"){
            PrintActionsLog* print = new PrintActionsLog();
            return print;
        }

        if(com == "backup"){
            BackupStudio* back = new BackupStudio();
            actionsLog.push_back(back);
            return back;
        }

        if (com == "restore"){
            RestoreStudio* rest = new RestoreStudio();
            actionsLog.push_back(rest);
            return rest;
        }
        return nullptr;
    }

    void Studio:: start(){
        cout <<"Studio is now open!" << endl;
        open = true;
        while (true){
            char command [500];
            cin.getline(command,500);
            BaseAction* action = buildAction(command);
            action->act(*this);
            if(strcmp(command,"closeall")==0)
                break;
        }
    }

    int Studio:: getNumOfTrainers() const{
        return numOfTrainers;
    }


    Trainer* Studio:: getTrainer(int tid){
        for (Trainer* t : trainers) {
            if (t->getId() == tid)
                return t;
        }
        return nullptr;
    }

    const std::vector<BaseAction *>& Studio:: getActionsLog() const {// Return a reference to the history of actions
        return actionsLog;
    }

    std::vector <Workout> & Studio::getWorkoutOptions(){
         return workout_options;
    }

    void Studio::closeStudio(){
        open = false;
    }

    int Studio::getCustomersId(){
        return customersId;
    }

    void Studio::addAction(BaseAction* action){
        actionsLog.push_back(action);
    }


