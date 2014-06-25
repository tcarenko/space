#pragma once
#ifndef NOXIM_RL_OPTS_H
#define NOXIM_RL_OPTS_H

#include <vector>

class NoximRLOpts{
public:
	static NoximRLOpts& getInstance(){
		static NoximRLOpts noximRLOpts;
		return noximRLOpts;
	}
	unsigned int actionsNum;
	vector<double> ranges;
	vector<unsigned int> numOfChunks;

private:
	NoximRLOpts(){}
	NoximRLOpts(NoximRLOpts const&);
	void operator=(NoximRLOpts const&);
};

#endif