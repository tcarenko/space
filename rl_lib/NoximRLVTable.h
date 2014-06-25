#pragma once
#ifndef NOXIM_RL_VISIT_H
#define NOXIM_RL_VISIT_H

#include "RLTable.h"
#include <map>
#include <vector>

class NoximRLVTable : RL::RLTable<unsigned int> {
public:
	NoximRLVTable();
	//override
	virtual void put(std::vector<unsigned int>) override;
	//override
	virtual std::vector<unsigned int> get() override;
	//
	void setStateAction(std::pair<unsigned int, unsigned int>);
	
	
private:
	std::pair<unsigned int, unsigned int> stateAction;
	//this is a map
	//the first value is a pair of numbers representing the state-action
	//the second number is the number of visits
	std::map<std::pair<unsigned int, unsigned int>, unsigned int> vTable;
};

#endif