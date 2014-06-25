#pragma once
#ifndef NOXIM_RL_SYSTEM_H
#define NOXIM_RL_SYSTEM_H


#include "RLSystem.h"
#include "NoximRLAgent.h"
#include "NoximRLState.h"
#include "NoximRLDiscreteAction.h"
#include "NoximRLOpts.h"
#include <vector>

template<typename Ty1, 
	typename Ty2,
	typename Ty3>
class NoximRLSystem : RL::RLSystem{

public:
	NoximRLSystem();
	//override
	void nextIteration();
	//sets the current state from the outer world
	void setState(NoximRLState<Ty3>);
	//sets the reward from the outer world
	void setFeedback(vector<double>);
	//returns the chosen action to the outer world
	NoximRLDiscreteAction<Ty1>* getAction();

private:
	NoximRLAgent<Ty1,Ty2,Ty3> agent;
	NoximRLDiscreteAction<Ty1> action;
	unsigned int tick;
};

template<typename Ty1, 
	typename Ty2,
	typename Ty3> 
NoximRLSystem<Ty1,Ty2,Ty3>::NoximRLSystem() : action(NoximRLOpts::getInstance().actionsNum, NoximRLOpts::getInstance().numOfChunks, NoximRLOpts::getInstance().ranges){
	tick = 0;
}

template<typename Ty1, 
	typename Ty2,
	typename Ty3> 
void NoximRLSystem<Ty1,Ty2,Ty3>::nextIteration(){
	this ->action = *(NoximRLDiscreteAction<Ty1>*)this ->agent.act();
	tick++;
}

template<typename Ty1, 
	typename Ty2,
	typename Ty3> 
void NoximRLSystem<Ty1,Ty2,Ty3>::setState(NoximRLState<Ty3> state){
	this ->agent.setState(state);
}

template<typename Ty1, 
	typename Ty2,
	typename Ty3> 
void NoximRLSystem<Ty1,Ty2,Ty3>::setFeedback(vector<double> feedback){
	this ->agent.setFeedback(feedback);
}

template<typename Ty1, 
	typename Ty2,
	typename Ty3> 
NoximRLDiscreteAction<Ty1>* NoximRLSystem<Ty1,Ty2,Ty3>::getAction(){
	return &this ->action;
}

#endif