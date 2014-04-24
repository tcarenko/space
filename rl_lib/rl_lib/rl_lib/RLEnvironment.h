#pragma once
#ifndef RL_ENVIRONMENT_H
#define RL_ENVIRONMENT_H

#include "RLState.h"
namespace RL {
	///RLExperiment pure abstract class
	class RLEnvironment {

	public:

		/* to estimate the current state of the agent and return reward
		*/
		virtual double giveFeedback(RLState* const ) = 0;

		/* experiment dictates the number of possible actions in each state
		*/
		virtual void setActionsNum(unsigned int) = 0;
	};
}

#endif