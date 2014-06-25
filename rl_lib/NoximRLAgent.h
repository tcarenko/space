#pragma once
#ifndef NOXIM_RL_AGENT_H
#define NOXIM_RL_AGENT_H

#include "RLAgent.h"
#include "RLAction.h"
#include "NoximRLAction.h"
#include "NoximRLState.h"
#include "NoximRLLearning.h"
#include "NoximRLDiscreteAction.h"
#include "NoximRLOpts.h"
#include <stdlib.h>
#include <time.h>
#include <vector>



template<typename Ty1, 
	typename Ty2,
	typename Ty3>
class NoximRLAgent : RL::RLAgent{
public:
	//constructor set the number of actions
	NoximRLAgent();
	//act method - the main one
	virtual RL::RLAction* act() override;
	//set params(features) from the outer world
	void setState(NoximRLState<Ty3>);
	//set reward from the outer world
	void setFeedback(vector<double>);
private:
	NoximRLState<Ty3> state;
	
	NoximRLLearning<Ty1, Ty2, Ty3> apprQLearning;
	//choose the action
	NoximRLDiscreteAction<Ty1>* chooseAction(unsigned int);
};


#define MAX_GEN_RL			100
#define THRESH_RL			2	
#define EPSILON_RL			MAX_GEN_RL/THRESH_RL


template<typename Ty1, 
	typename Ty2,
	typename Ty3> 
NoximRLAgent<Ty1,Ty2,Ty3>::NoximRLAgent(){
}

template<typename Ty1, 
	typename Ty2,
	typename Ty3> 
RL::RLAction* NoximRLAgent<Ty1,Ty2,Ty3>::act(){
	//here according to a certain state <cm, appsDens>, agent needs to choose an action
	//it might be the best action (with the maximum Q), or with some probability p agent might choose a worse action
	NoximRLDiscreteAction<Ty1>* action = this ->chooseAction((unsigned int)EPSILON_RL);
	//agents learns while acting
	this ->apprQLearning.setAction(action);
	//update set of weights
	this ->apprQLearning.update();
	return (RL::RLAction*)action;
}

template<typename Ty1, 
	typename Ty2,
	typename Ty3> 
void NoximRLAgent<Ty1,Ty2,Ty3>::setState(NoximRLState<Ty3> state){
	this ->state = state;
	this ->apprQLearning.setState(state);
}

template<typename Ty1, 
	typename Ty2,
	typename Ty3>
void NoximRLAgent<Ty1,Ty2,Ty3>::setFeedback(vector<double> reward){
	this ->apprQLearning.setFeedback(reward);
}

template<typename Ty1, 
	typename Ty2,
	typename Ty3> 
NoximRLDiscreteAction<Ty1>* NoximRLAgent<Ty1,Ty2,Ty3>::chooseAction(unsigned int epsilon){
	//choose the best action at the current state. if the table is empty the returned action will be empty
	NoximRLDiscreteAction<Ty1>* action = this ->apprQLearning.getBestAction();
	//exploitation/exploration should be introduced here
	//for now we introduce epsilon constant, which defines the maximum probability with which the best action will be taken
	srand(time(NULL));
	//generate random number from 1 to 100
	float p = (rand()%MAX_GEN_RL + 1);
	//check if we should take a random action or the best one
	//epsilon should be less than MAX_GEN
	if((unsigned int)p < epsilon || ((NoximRLAction<Ty1>*)action) ->isZero()){
		//take a random action - the best action is a reference point for that
		vector<Ty1> contVals;
		contVals.resize(action ->getParams().size());
		for(int i = 0; i < action ->getParams().size(); i++){
			//to choose a random action we just generate a random number from 0 to the numberOfChunks for each parameter of the action
			unsigned int t = rand()%action ->getNumberOfChunks()[i];
			contVals[i] = t * action ->getRanges()[i]/action ->getNumberOfChunks()[i];
		}
		//now we have a new vector of continuous values
		action ->setParams(contVals);
	}

	//take the best action
	return action;	
}


#endif