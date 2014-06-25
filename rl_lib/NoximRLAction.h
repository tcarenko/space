#pragma once
#ifndef NOXIM_RL_ACTION_H
#define NOXIM_RL_ACTION_H

#include "RLAction.h"
#include <vector>
#include <algorithm>

template <typename T> 
bool zeroCond(T _val){
	return _val == (T)0;
}

template <typename T>
class NoximRLAction : RL::RLAction {
public:
	NoximRLAction(unsigned int);
	//override from the base class
	bool operator==(const RLAction&);
	//set params
	virtual void setParams(const std::vector<T>);
	//get params
	virtual std::vector<T> getParams();
	//check if it is zero action
	bool isZero();
protected:
	std::vector<T> params;
};

template <typename T> NoximRLAction<T>::NoximRLAction(unsigned int size){
	this ->params.clear();
	this ->params.resize(size);
}

template <typename T> bool NoximRLAction<T>::operator==(const RL::RLAction& val){
	return (this ->params == ((NoximRLAction&)val).params);
}

template <typename T> void NoximRLAction<T>::setParams(const std::vector<T> params){
	this ->params = params;
}

template <typename T> std::vector<T> NoximRLAction<T>::getParams(){
	return this ->params;
}

template <typename T> bool NoximRLAction<T>::isZero(){
	return std::all_of(this ->params.begin(), this ->params.end(), zeroCond<T>);
}

#endif