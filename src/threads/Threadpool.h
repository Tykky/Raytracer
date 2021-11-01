#ifndef RAYTRACER_THREADPOOL_H
#define RAYTRACER_THREADPOOL_H

#include <queue>
#include <mutex>

#include "samplers/Sampler.h"

class Sampler;

struct Task
{
    int x0, y0; // Starting points
    int x1, y1; // width and height
    int sampleCount;
    int sampleTarget;
};

class Threadpool
{
public:
    Threadpool(Sampler* sampler) :
        m_sampler(sampler)
    {}
    void push(Task* task);
    Task* pop();

    bool isEmpty() const;
    void spawnThreads();
    void executeThread();
    void killThreads();

private:
    std::queue<Task*>        m_queue;
    std::mutex               m_mutex;
    std::vector<std::thread> m_threads;
    Sampler*                 m_sampler;
    unsigned int             m_sleepDuration = 200;
    bool                     m_stop = false;
};

typedef std::vector<Task> Taskbuffer;

#endif //RAYTRACER_THREADPOOL_H
