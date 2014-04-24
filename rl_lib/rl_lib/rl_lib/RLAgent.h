#pragma once
#ifndef RL_AGENT_H
#define RL_AGENT_H

#include "RLAction.h"
#include "RLState.h"

namespace RL {
	///RLAgent pure abstract class
	class RLAgent {
	public:
		virtual RLState*  act(RLAction* const) = 0;
		virtual RLState*  getNextState() = 0;
		virtual RLAction* getAction(RLState* const) = 0;
	};
	
}

#endif