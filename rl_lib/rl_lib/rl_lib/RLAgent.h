#pragma once
#ifndef RL_AGENT_H
#define RL_AGENT_H

#include "RLAction.h"

namespace RL {
	///RLAgent pure abstract class
	class RLAgent {
	public:
		void act(const RLAction*);
	};
}

#endif