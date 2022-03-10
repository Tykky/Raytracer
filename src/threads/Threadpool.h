#ifndef RAYTRACER_THREADPOOL_H
#define RAYTRACER_THREADPOOL_H

#include <queue>
#include <mutex>
#include <thread>

#include "core/buffers.h"

class Threadpool;
struct Task;

#include "samplers/Sampler.h"

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
    bool renderFinished() const;
    void spawnThreads();
    void killThreads();

    std::function<void()> renderFinishedCallback = nullptr;

private:
    void executeThread();

    std::queue<Task*>        m_queue;
    std::mutex               m_mutex;
    std::vector<std::thread> m_threads;
    Sampler*                 m_sampler;
    unsigned int             m_sleepDuration = 200;
    bool                     m_stop = false;
    bool                     m_renderFinished = false;
};

#endif //RAYTRACER_THREADPOOL_H;
