#pragma once
#ifndef NOXIM_RL_STATE_H
#define NOXIM_RL_STATE_H

#include "RLState.h"
#include "NoximApplication.h"
#include <vector>
#include <functional>

template<typename T = double>
class NoximRLState : RL::RLState{

public:
	bool operator==(const RLState&);
	//here is some hash function implementation
	operator size_t () const;
	//calculate the Q value as a multiplication of 2 vectors
	double getQ(std::vector<double>);
	//features
	std::vector<T> features;
private:
	
};

template<typename T> bool  NoximRLState<T>::operator==(const RLState& val){
		return (this ->features == ((NoximRLState<T>&)val).features);
	}

template<typename T> NoximRLState<T>::operator size_t () const {
	std::hash<double> hash;
	std::size_t result = 0;
	for(int i = 0; i < this ->features.size(); i++){
		result ^= hash(this ->features[i]);
	}
	return result;
}

template<typename T> double NoximRLState<T>::getQ(std::vector<double> w){
	double q = 0.0;
	//if the size of the features is 0 - return 0
	if(this ->features.size() == 0) return 0.0;
	for(int i = 0; i < w.size(); i++)
		q += w[i]* this->features[i];
	return q;
}


#endif