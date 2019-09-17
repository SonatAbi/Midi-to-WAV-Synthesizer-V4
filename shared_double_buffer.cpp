#include "shared_double_buffer.hpp"

// Implement your shared double buffer here

void shared_double_buffer::push(double const & message)
{
	std::unique_lock<std::mutex> lock(the_mutex);
	wQueue.push(message);
	lock.unlock();
	the_condition_variable.notify_one();
}

bool shared_double_buffer::empty() const
{
	std::lock_guard<std::mutex> lock(the_mutex);
	return rQueue.empty();
}

void shared_double_buffer::swap()
{
	std::unique_lock<std::mutex> lock(the_mutex);
	rQueue.swap(wQueue);
	
	lock.unlock();
	the_condition_variable.notify_one();
}

bool shared_double_buffer::try_pop(double & popped_value)
{
	if (rQueue.empty())
	{
		return false;
	}

	popped_value = rQueue.front();
	rQueue.pop();
	return true;
}

void shared_double_buffer::reset()
{
	while (wQueue.size() > 0) {
		wQueue.pop();
	}
	while (rQueue.size() > 0) {
		rQueue.pop();
	}
}


