#pragma once
#ifndef NOXIM_RL_ENVIRONMENT_H
#define NOXIM_RL_ENVIRONMENT_H

#include "RLEnvironment.h"
#include "RLAction.h"
#include "NoximRLDiscreteAction.h"
#include "NoximRLState.h"
#include "NoximRLSystem.h"
#include "NoximMain.h"
#include <vector>
#include <stdlib.h>
#include <time.h>

template<typename Ty1, 
	typename Ty2, 
	typename Ty3>
class NoximRLEnvironment : RL::RLEnvironment{

public:
	NoximRLEnvironment();
	//override. called by the RL to get the feedback
	double getFeedback(RL::RLState* const);

	//override. called by the outer code
	RL::RLAction* getAction();

	//the method is called by the outer code
	void setState(std::vector<double>);

	//the method is called by the outer code
	void setFeedback(std::vector<double>);

	//returns the NoximID node
	NoximID getNode();

private:
	vector<double> feedback;
	NoximRLState<Ty3> rlState;
	NoximRLSystem<Ty1,Ty2,Ty3> rlSystem;

};

template<typename Ty1, 
	typename Ty2, 
	typename Ty3> 
NoximRLEnvironment<Ty1,Ty2,Ty3>::NoximRLEnvironment(){
}

template<typename Ty1, 
	typename Ty2, 
	typename Ty3> 
double NoximRLEnvironment<Ty1,Ty2,Ty3>::getFeedback(RL::RLState* const state){
	//here we need to calculate NMRD of the mapped application 
	//and the fragmentation degree of the remained resources
	return 1/feedback[0];
}

template<typename Ty1, 
	typename Ty2, 
	typename Ty3> 
RL::RLAction* NoximRLEnvironment<Ty1,Ty2,Ty3>::getAction(){
	//make the next iteration - will happen a posteriori
	this ->rlSystem.nextIteration();
	//retrive the action from the system
	return (RL::RLAction*)rlSystem.getAction();
}

template<typename Ty1, 
	typename Ty2, 
	typename Ty3> 
void NoximRLEnvironment<Ty1,Ty2,Ty3>::setFeedback(std::vector<double> fdb){
	//set the feedback
	this ->feedback = fdb;
	this ->rlSystem.setFeedback(fdb);
}

template<typename Ty1, 
	typename Ty2, 
	typename Ty3> 
void NoximRLEnvironment<Ty1,Ty2,Ty3>::setState(std::vector<double> features){
	//set the new state
	this ->rlState.features = features;
	//get NoximRLState out of features
	NoximRLState<Ty3> state;
	state.features = features;
	this ->rlSystem.setState(state);
}

template<typename Ty1, 
	typename Ty2, 
	typename Ty3> 
NoximID NoximRLEnvironment<Ty1,Ty2,Ty3>::getNode(){
	//conversion from state,action to node
	NoximID node;
	NoximRLDiscreteAction<Ty1>* action = (NoximRLDiscreteAction<Ty1>*)this ->getAction();
	//we got an action from the RLSystem - apply it to get a node finally
	



	return node;
}

#endif