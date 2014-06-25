#pragma once
#ifndef NOXIM_RL_LEARNING_H
#define NOXIM_RL_LEARNING_H

#include "RLLearning.h"
#include "RLState.h"
#include "RLAction.h"
#include "NoximRLState.h"
#include "NoximRLDiscreteAction.h"
#include "NoximRLWTable.h"
#include "NoximRLVTable.h"
#include "NoximRLOpts.h"
#include <vector>

#define NOXIM_RL_ALPHA		0.1l
#define NOXIM_RL_GAMMA		0.1l
#define NOXIM_RL_ACT_NUM	3u

template <typename Ty1, 
	typename Ty2,
	typename Ty3>
class NoximRLLearning : RL::RLLearning{
public:
	NoximRLLearning();
	//override
	virtual void update() override;
	//set current parameters (variables)
	void setState(NoximRLState<Ty3>);
	//set obtained reward
	void setFeedback(vector<double>);
	//set action
	void setAction(NoximRLDiscreteAction<Ty1>*);
	//returns best action
	NoximRLDiscreteAction<Ty1>* getBestAction();
	//returns the visit count
	unsigned int getVisitCount(NoximRLState<Ty3>, NoximRLDiscreteAction<Ty1>);

private:
	//vector of params from the environment, THE CURRENT STATE
	NoximRLState<Ty3> state;
	//the previous state
	NoximRLState<Ty3> prevState;
	//chosen action from THE PREVIOUS STATE!
	NoximRLDiscreteAction<Ty1> action;
	//number of parameters
	unsigned int paramsNum;
	//reward from environment
	vector<double> reward;
	//alpha param
	double alpha;
	//gamma param
	double gamma;
	//returns Qmax
	double getQmax();
	//calculates overall reward
	double getRwrd();
	//w table. weights table
	NoximRLWTable<Ty2,Ty1,Ty3> wTable;
	//v table. visit table
	NoximRLVTable vTable;

};

template <typename Ty1, 
	typename Ty2,
	typename Ty3> 
NoximRLLearning<Ty1,Ty2,Ty3>::NoximRLLearning() : action(NoximRLOpts::getInstance().actionsNum, 
																					NoximRLOpts::getInstance().numOfChunks, 
																					NoximRLOpts::getInstance().ranges){
	this ->reward.clear();
	this ->alpha = NOXIM_RL_ALPHA;
	this ->gamma = NOXIM_RL_GAMMA;
	this ->paramsNum = NOXIM_RL_ACT_NUM;

	this ->prevState.features.clear();
	this ->state.features.clear();
}

template <typename Ty1, 
	typename Ty2,
	typename Ty3> 
void NoximRLLearning<Ty1,Ty2,Ty3>::update(){
	//here we should update the weights of a certain action
	//set an action
	wTable.setAction(this ->action);
	//fetch the w vector. check if the table is empty or not

	vector<Ty2> weights = wTable.get();
	//check if w vector is empty
	if(weights.empty())
		//it is empty - the action has not been taken yet - means w should be initialized with 0s
		for(int i =0; i < this ->state.features.size(); i++)
			weights.push_back(0.0);
	//get qmax
	double qMax = this ->getQmax();
	wTable.setAction(this ->action);
	//diff = (r + gamma*Qmax)-Q -> Q here is the value, assgned to the previous state with the current action
	double diff = (this ->getRwrd() + this ->gamma * this ->getQmax()) - this ->prevState.getQ(weights);
	//recalc new weights
	for(int i = 0; i < this ->state.features.size(); i++){
		if(weights.size() == i) weights.push_back(this ->alpha * diff * state.features[i]);
		else weights[i] = weights[i] + this ->alpha * diff * state.features[i];
	}
	//put the updated vector of weights back to the wTable
	wTable.put(weights);
	//rewrite the previous state
	this ->prevState = this ->state;
}

template <typename Ty1, 
	typename Ty2,
	typename Ty3> 
void NoximRLLearning<Ty1,Ty2,Ty3>::setState(NoximRLState<Ty3> state){
	this ->state = state;
}

template <typename Ty1, 
	typename Ty2,
	typename Ty3> 
void NoximRLLearning<Ty1,Ty2,Ty3>::setAction(NoximRLDiscreteAction<Ty1>* action){
	this ->action = *action;
}

template <typename Ty1, 
	typename Ty2,
	typename Ty3> 
void NoximRLLearning<Ty1,Ty2,Ty3>::setFeedback(vector<double> feedback){
	this ->reward = feedback;
}

template <typename Ty1, 
	typename Ty2,
	typename Ty3> 
double NoximRLLearning<Ty1,Ty2,Ty3>::getQmax(){
	//the hardest part. choose 
	double qMax = 0.0;
	//get action with the max Q
	NoximRLDiscreteAction<Ty1>* maxAction = this ->getBestAction();
	//get w vector
	wTable.setAction(*maxAction);
	vector<double> w = wTable.get();
	//get Q
	qMax = this ->state.getQ(w);
	return qMax;
}

template <typename Ty1, 
	typename Ty2,
	typename Ty3> 
NoximRLDiscreteAction<Ty1>* NoximRLLearning<Ty1,Ty2,Ty3>::getBestAction(){
	//get action with the max Q
	return wTable.getMaxQAction(this ->state);
}

//template <typename Ty1> unsigned int NoximRLLearning::getVisitCount(NoximRLState vState, NoximRLDiscreteAction vAction){
//	std::vector<unsigned int> vCount;
//	vCount.clear();
//	// get state-action
//	size_t state_t = (size_t)vState;
//	size_t action_t = (size_t)vAction;
//	std::pair<unsigned int, unsigned int> stateAction = std::make_pair(state_t, action_t);
//	//set state-action in a vTable
//	vTable.setStateAction(stateAction);
//	//get count
//	vCount = vTable.get();
//	return vCount[0];
//}

template <typename Ty1, 
	typename Ty2,
	typename Ty3> 
double NoximRLLearning<Ty1,Ty2,Ty3>::getRwrd(){
	double rwrd = 0.0;
	for(int i = 0; i < this ->reward.size(); i++)
		rwrd += this ->reward[i];
	return rwrd;
}

#endif