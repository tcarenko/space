#pragma once
#ifndef RL_LEARNING_H
#define RL_LEARNING_H

#include <vector>

namespace RL {
	///RLLearning prue abstract class
	//due to different types of learning, this class can incapsulate a QTable or a feature vector
	class RLLearning{
	public:

		/* updates the given state/action with a new q value
			based on the type of learning, the update function will differ
		*/
		virtual void update() = 0;
	};
}
#endif