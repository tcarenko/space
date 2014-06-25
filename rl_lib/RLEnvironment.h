#pragma once
#ifndef RL_ENVIRONMENT_H
#define RL_ENVIRONMENT_H

#include "RLState.h"
#include "RLAction.h"

namespace RL {
	///RLExperiment pure abstract class
	class RLEnvironment {

	public:
		/* to estimate the current state of the agent and return reward
		*/
		virtual double getFeedback(RLState* const ) = 0;

		/*function is called by the outer code to make the next step*/
		virtual RLAction* getAction() = 0;
		
	};
}

#endif