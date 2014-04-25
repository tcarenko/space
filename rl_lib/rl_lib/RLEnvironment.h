#pragma once
#ifndef RL_ENVIRONMENT_H
#define RL_ENVIRONMENT_H

#include "RLState.h"
#include "RLAction.h"

namespace RL {
	///RLExperiment pure abstract class
	class RLEnvironment {

	public:
		/*environment's job is to return the state, the action leads to
		*/
		virtual RLState* getState(RLAction* const) = 0;
		
		/* to estimate the current state of the agent and return reward
		*/
		virtual double getFeedback(RLState* const ) = 0;

		/* experiment dictates the number of possible actions in each state
		*/
		virtual void setActionsNum(unsigned int) = 0;
	};
}

#endif