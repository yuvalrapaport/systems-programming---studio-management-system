#ifndef STUDIO_H_
#define STUDIO_H_

#include "Action.h"
#include "Customer.h"
#include "Workout.h"
#include "Trainer.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <istream>

using namespace std;
//Forward declaration
class BaseAction;

class Studio{		
public:
	Studio();
    Studio(const std::string &configFilePath);
    Studio(const Studio &other); //copy constructor
    Studio(Studio &&other); //move constructor
    Studio& operator= (const Studio &other); //copy assignment operator
    Studio& operator= (Studio &&other); //move assignment operator
    virtual ~Studio(); //destructor
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    void closeStudio(); //added
    BaseAction* buildAction(char* command); //added
    int getCustomersId(); //added
    void addAction (BaseAction*);//added

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    int customersId; //added
    int numOfTrainers; //added
};

#endif