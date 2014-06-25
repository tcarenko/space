#include "NoximRLVTable.h"

NoximRLVTable::NoximRLVTable(){
	vTable.clear();
}

//override
void NoximRLVTable::put(std::vector<unsigned int> toPut){
	//toPut contains 2 params - state, action
	//new pair
	std::pair<unsigned int, unsigned int> stateAction;
	stateAction.first = toPut[0];
	stateAction.second = toPut[1];

	//check if this state-action has been visited previously
	if(!this ->vTable.count(stateAction)) this ->vTable.insert(std::pair<std::pair<unsigned int, unsigned int>, unsigned int>(stateAction, 0));
	//it has been visited - increase the visit number
	else this ->vTable[stateAction]++;
}

//override
std::vector<unsigned int> NoximRLVTable::get(){
	std::vector<unsigned int> v;
	v.clear();
	//if there is the action in a table
	if(vTable.count(this ->stateAction)) {
		v.push_back(this ->vTable[this ->stateAction]);
		return v;
	}
	return v;
}

void NoximRLVTable::setStateAction(std::pair<unsigned int, unsigned int> stateAction){
	this ->stateAction = stateAction;
}