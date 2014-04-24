#pragma once
#ifndef RL_SYSTEM_H
#define RL_SYSTEM_H

#include "RLEnvironment.h"
#include "RLAgent.h"
#include "RLLearning.h"
#include "RLQtable.h"

namespace RL {
	///RLEnvironment pure abstract class
	class RLSystem {

	public:

		/*to introduce a notion of time
		*/
		virtual void nextIteration() = 0;

		/*to bind a specific experiment
		*/
		virtual void setExperiment(RLEnvironment* const) = 0;

	protected:
		RLEnvironment* experiment;
		RLAgent* agent;
		RLLearning* learning;
		RLQtable* qTable;


		unsigned int tick;
	};

}
#endif