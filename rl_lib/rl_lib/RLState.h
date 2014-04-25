#pragma once
#ifndef RL_STATE_H
#define RL_STATE_H

namespace RL {
	///RLState pure abstract class
	class RLState{

	public:
		virtual bool operator==(const RLState&) = 0;
	};
}

#endif