#pragma once
#ifndef RL_LEARNING_H
#define RL_LEARNING_H

namespace RL {
	///RLLearning prue abstract class
	class RLLearning{
	public:

		/* updates the given state/action with a new q value
		*/
		virtual double update(const RLState*, const RLState*, const RLAction*, double) = 0;
	};
}
#endif