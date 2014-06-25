#pragma once
#ifndef NOXIM_RL_Q_TABLE
#define NOIXM_RL_Q_TABLE

#include "RLTable.h"
#include <vector>
#include <map>
#include "NoximRLDiscreteAction.h"
#include "NoximRLState.h"
#include "NoximRLOpts.h"

template <typename Ty1,
	typename Ty2,
	typename Ty3>
class NoximRLWTable : RL::RLTable<Ty1> {
public:
	NoximRLWTable();
	//override
	virtual void put(std::vector<Ty1>) override;
	//override
	virtual std::vector<Ty1> get() override;
	//get the action which returns the max Q at the certain state
	NoximRLDiscreteAction<Ty2>* getMaxQAction(NoximRLState<Ty3>);
	//set an action
	void setAction(NoximRLDiscreteAction<Ty2>);
private:
	//this is a map
	//first value is an action
	//second value is a vector of weights
	std::map<NoximRLDiscreteAction<Ty2>, std::vector<Ty1>> wTable;
	//action we work with
	NoximRLDiscreteAction<Ty2> action;
};


template <typename Ty1,
	typename Ty2,
	typename Ty3> 
NoximRLWTable<Ty1,Ty2,Ty3>::NoximRLWTable() : action(NoximRLOpts::getInstance().actionsNum, 
																				NoximRLOpts::getInstance().numOfChunks, 
																				NoximRLOpts::getInstance().ranges){
	wTable.clear();
}

template <typename Ty1,
	typename Ty2,
	typename Ty3> 
void NoximRLWTable<Ty1,Ty2,Ty3>::put(std::vector<Ty1> w){
	//check if the element is already in the table
	if(!wTable.count(this ->action)) wTable.insert(std::make_pair(this ->action, w));
	else wTable[action] = w;
	
}

template <typename Ty1,
	typename Ty2,
	typename Ty3> 
std::vector<Ty1> NoximRLWTable<Ty1,Ty2,Ty3>::get(){
	std::vector<Ty1> w;
	w.clear();
	//if there is the action is a table
	if(wTable.count(this ->action)) 
		return wTable[this ->action];
	else
		return w;
}

template <typename Ty1,
	typename Ty2,
	typename Ty3> 
void NoximRLWTable<Ty1,Ty2,Ty3>::setAction(NoximRLDiscreteAction<Ty2> action){
	this ->action = action;
}

template <typename Ty1,
	typename Ty2,
	typename Ty3> 
NoximRLDiscreteAction<Ty2>* NoximRLWTable<Ty1,Ty2,Ty3>::getMaxQAction(NoximRLState<Ty3> state){
	double qMax = 0.0;
	NoximRLDiscreteAction<Ty2>* action = new NoximRLDiscreteAction<Ty2>(NoximRLOpts::getInstance().actionsNum, NoximRLOpts::getInstance().numOfChunks, NoximRLOpts::getInstance().ranges);
	//we need to check it the table is empty!
	if(!this ->wTable.empty()){
		//get iterator
		std::map<NoximRLDiscreteAction<Ty2>, std::vector<Ty1>>::iterator it = this ->wTable.begin();
		//initialize the action
		*action = it ->first;
		//iterate!
		for(;it != wTable.end(); it++){
			//get Q
			double qTemp = 0.0;
			qTemp = state.getQ(it ->second);
			//check if its the new max
			if(qTemp > qMax){
				//new max value
				qMax = qTemp;
				*action = it ->first;
			}
		}
	}
	return action;
}

#endif