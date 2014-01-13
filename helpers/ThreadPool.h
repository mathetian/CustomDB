#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H


#include <mutex>
#include <queue>
#include <memory>
#include <chrono>
#include <thread>
#include <vector>
#include <functional>
#include <condition_variable>
using namespace std;

#include <assert.h>

/**
	Use C++0x thread, mutex, lock.
**/

class WorkThread;
class ThreadPool;

typedef shared_ptr<WorkThread> SWork;
typedef shared_ptr<ThreadPool> SPool;
typedef shared_ptr<thread>     SThread;

class ThreadPool : public enable_shared_from_this<ThreadPool>{
private:
    typedef function<void()> Task;

public:
    ThreadPool(int threadNum = 5);

   ~ThreadPool();

public:
  void schedule(const Task & task);

  void wait() const;

  /**Without implementation, can't understand the format for time.**/
  void timedwait(struct timeval & spec) const;

  void timedwait(chrono::steady_clock::time_point abs_time) const;

  bool resize(int targetNum = 0);

public:
  int  activeNum() const { return m_actThreadNum; }
  
  int  pendingNum() const { return m_Tasks.size(); }

  void clear() { }

  bool empty() { return m_Tasks.size() == 0 ? 1 : 0; }

private:
  void workerDestructed(SWork worker);

  bool executeTask();

  void killAllThreads();

private:
  int    m_threadNum;
  int    m_actThreadNum;
  int    m_targetNum;

private:
  queue  < Task> m_Tasks;
  vector <SWork> m_terminatedWorks;

private:
  mutable mutex              m_monitor;
  mutable condition_variable m_taskCIN;
  mutable condition_variable m_tasksFinishedOrTerminated;
  
  friend class WorkThread;

};

class WorkThread
{
public:
    static void createThread(SPool const & pool); 
   ~WorkThread() { }

public:
    void run() { while(m_pool -> executeTask()) {} }
    void join() { m_thread -> join(); }

private:
    WorkThread(SPool const & pool) : m_pool(pool) { assert(pool); }

private:
    SPool    m_pool;     
    SThread  m_thread; 
};

#endif