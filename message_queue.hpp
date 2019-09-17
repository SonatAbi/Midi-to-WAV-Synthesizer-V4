#ifndef MESSAGE_QUEUE_HPP
#define MESSAGE_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>


class message_queue
{
 public:

	 void push(std::string const& message);

	 bool empty() const;

	 bool try_pop(std::string & popped_value);

	 void wait_and_pop(std::string & popped_value);

 private:
	 std::queue<std::string> the_queue;
	 mutable std::mutex the_mutex;
	 std::condition_variable the_condition_variable;
};

#endif
