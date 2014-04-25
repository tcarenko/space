#pragma once
#ifndef RL_AGENT_H
#define RL_AGENT_H

#include "RLAction.h"
#include "RLState.h"

namespace RL {
	///RLAgent pure abstract class
	class RLAgent {
	public:
		// agent's job is to act, which means to choose the next action, based on the state it is at
		virtual RLAction* act(RLState* const) = 0;
		virtual RLState*  getNextState() = 0;
		virtual RLAction* getAction(RLState* const) = 0;
	};
	
}

#endif