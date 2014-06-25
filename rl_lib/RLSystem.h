#pragma once
#ifndef RL_SYSTEM_H
#define RL_SYSTEM_H

#include "RLEnvironment.h"
#include "RLAgent.h"
#include "RLLearning.h"
#include "RLState.h"

namespace RL {
	///RLEnvironment pure abstract class
	class RLSystem {

	public:

		/*to introduce a notion of time
		*/
		virtual void nextIteration() = 0;


	/*protected:
		RLEnvironment* environment;
		RLAgent*       agent;
		RLLearning*    learning;


		unsigned int tick;*/
	};

}
#endif