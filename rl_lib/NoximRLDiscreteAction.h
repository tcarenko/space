#pragma once
#ifndef NOXIM_DISCRETIZER_ACTION_H
#define NOXIM_DISCRETIZER_ACTION_H

#include "NoximDiscretizer.h"
#include "NoximRLAction.h"
#include "RLAction.h"
#include <vector>

template <typename T>
class NoximRLDiscreteAction : public NoximRLAction<T>, NoximDiscretizer {
public:
	NoximRLDiscreteAction(unsigned int, std::vector<unsigned int>, std::vector<double> );
	//override
	virtual void setNumberOfChunks(const std::vector<unsigned int>) override;
	std::vector<unsigned int> getNumberOfChunks();
	//override
	virtual void setRanges(const std::vector<double>) override;
	std::vector<double> getRanges();
	//override the set function of the NoximRLAction
	virtual void setParams(const std::vector<T>) override;
	//override the get function of the NoximRLAction
	virtual std::vector<T> getParams() override;
	//need also to override "<" operator to be able to use this class in an std::map
	bool operator <(const NoximRLDiscreteAction& val) const;
	//override = operator
	//size_t override for some hash function implementation
	operator size_t () const;
private:
	///number of slots we divide rahnges to
	std::vector<unsigned int> chunks;
	///our ranges
	std::vector<double> ranges;
	std::vector<unsigned int> discrParams;
};

template <typename T> NoximRLDiscreteAction<T>::NoximRLDiscreteAction(unsigned int actionsParamSize, std::vector<unsigned int> numOfChunks, std::vector<double> ranges) : NoximRLAction(actionsParamSize), chunks(numOfChunks), ranges(ranges){
}

template <typename T> void NoximRLDiscreteAction<T>::setNumberOfChunks(const std::vector<unsigned int> chunks){
	this ->chunks = chunks;
}

template <typename T> void NoximRLDiscreteAction<T>::setRanges(const std::vector<double> ranges){
	this ->ranges = ranges;
}

//overriden
template <typename T> void NoximRLDiscreteAction<T>::setParams(const std::vector<T> params){
	this ->params = params;

	std::vector<unsigned int> discrParams;
	discrParams.resize(this ->getParams().size());

	for(int i = 0; i < this ->getParams().size(); i++){
		double up = 0.0;
		double step = this ->ranges[i]/this ->chunks[i];
		while(this ->getParams()[i] > up){
			up += step;
		}
		discrParams[i] = static_cast<unsigned int>(up);
	}
	this ->discrParams = discrParams;
}

template <typename T> std::vector<T> NoximRLDiscreteAction<T>::getParams(){
	return this ->params;
}

//std::vector<unsigned int> NoximRLDiscreteAction::getDiscreteVals(){
//	return this ->discrParams;
//}


template <typename T> bool NoximRLDiscreteAction<T>::operator<(const NoximRLDiscreteAction<T> &val) const{
	return this ->params[0] < val.params[0];
}

template <typename T> NoximRLDiscreteAction<T>::operator size_t() const{
	std::hash<unsigned int> hash;
	std::size_t result = 0;
	for(int i = 0; i < this ->discrParams.size(); i++){
		result ^= hash(this ->discrParams[i]);
	}
	return result;
}

template <typename T> std::vector<unsigned int> NoximRLDiscreteAction<T>::getNumberOfChunks(){
	return this ->chunks;
}

template <typename T> std::vector<double> NoximRLDiscreteAction<T>::getRanges(){
	return this ->ranges;
}
#endif