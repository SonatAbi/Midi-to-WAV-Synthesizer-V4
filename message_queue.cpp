#include "message_queue.hpp"


void message_queue::push(std::string const & message)
{
	std::unique_lock<std::mutex> lock(the_mutex);
	the_queue.push(message);
	lock.unlock();
	the_condition_variable.notify_one();
}


bool message_queue::empty() const
{
	std::lock_guard<std::mutex> lock(the_mutex);
	return the_queue.empty();
}


bool message_queue::try_pop(std::string & popped_value)
{
	std::lock_guard<std::mutex> lock(the_mutex);
	if (the_queue.empty())
	{
		return false;
	}

	popped_value = the_queue.front();
	the_queue.pop();
	return true;
}


void message_queue::wait_and_pop(std::string & popped_value)
{
	std::unique_lock<std::mutex> lock(the_mutex);
	while (the_queue.empty())
	{
		the_condition_variable.wait(lock);
	}

	popped_value = the_queue.front();
	the_queue.pop();
}
