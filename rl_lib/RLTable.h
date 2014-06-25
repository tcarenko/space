#pragma once
#ifndef RL_QTABLE_H
#define RL_QTABLE_H

#include <vector>

namespace RL {
	///RLQTable pure abstract class
	template <typename T> 
	class RLTable {
	public:
		/* puts value into the table
		*/
		virtual void put(std::vector<T>) = 0;

		/*gets value from the table
		*/
		virtual std::vector<T> get() = 0;
	};
}
#endif