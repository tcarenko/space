#pragma once
#ifndef RL_ACTION_H
#define RL_ACTION_H

namespace RL {
	///RLAction pure abstract class
	class RLAction{

	public:
		virtual bool operator== (const RLAction&) const = 0;
	};
}
#endif