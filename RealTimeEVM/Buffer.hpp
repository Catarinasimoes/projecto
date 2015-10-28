/**
* Real Time non-envasive heartbeat reader using "Eulerian Video Magnification" algorithm.
* Uses the OpenCV library and Microsoft Kinect One
* Based on the work of:
*      Jens Schindel		<kontakt@jens-schindel.de>
*      Joseph Pan			<https://github.com/wzpan/QtEVM>
*      Nick D'Ademo			<https://github.com/nickdademo/qt-opencv-multithreaded>
* @author Francisco Salgado <salgfrancisco@gmail.com>
* @version: 1.0
*/

#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <queue>
#include <condition_variable>	


//Thread safe queue implementation
template <typename T>
class Buffer
{
private:
	std::queue<T> queue;
	std::mutex queue_m;
	std::condition_variable cv;
	
public:
	Buffer(): queue(), queue_m(), cv() {
	}


	/**
	Adds data to queue. Blocks if another thread is using it;
	@param data Data to enqueue
	*/
	void enqueue(const T& data)
	{
		std::lock_guard<std::mutex> lock(queue_m);
		queue.push(data);
		cv.notify_one();
	}
	
	/**
	Returns first data object in queue and removes it. Blocks if another thread is using it or if queue is empty.
	@return T object
	*/
	T dequeue()	//if queue is empty, blocks until there is something
	{
		std::unique_lock<std::mutex> lock(queue_m);
		while (queue.empty())
		{
			cv.wait(lock);
		}
		T res = queue.front();
		queue.pop();
		return res;
	}

	/**
	Returns number of elements in queue.
	@return uint queue size
	*/
	unsigned int size()
	{
		std::lock_guard<std::mutex> lock(queue_m);
		unsigned int size = queue.size();
		cv.notify_one();
		return size;
	}
};

#endif