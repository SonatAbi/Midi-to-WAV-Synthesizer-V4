#ifndef SHARED_DOUBLE_BUFFER_HPP
#define SHARED_DOUBLE_BUFFER_HPP

#include <queue>
#include <mutex>
#include <condition_variable>


class shared_double_buffer
{
public:

	void push(double const& message);

	bool empty() const;

	void swap();

	bool try_pop(double & popped_value);

	void reset();

private:
	std::queue<double> wQueue;
	std::queue<double> rQueue;
	mutable std::mutex the_mutex;
	std::condition_variable the_condition_variable;
};

#endif