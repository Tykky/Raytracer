#include "Threadpool.h"

void Threadpool::push(Task task)
{
    std::scoped_lock lock(m_mutex);
    m_queue.push(task);
}

Task Threadpool::pop()
{
    std::scoped_lock lock(m_mutex);
    Task task = m_queue.front();
    m_queue.pop();
    return task;
}

bool Threadpool::isEmpty() const
{
    return m_queue.empty();
}

void Threadpool::spawnThreads()
{
    const unsigned int n = std::thread::hardware_concurrency();
    m_threads.resize(n);
    for (unsigned int i = 0; i < n; ++i)
    {
        m_threads[i] = std::thread([this]{ executeThread(); });
    }
}

void Threadpool::executeThread()
{
    while (!m_stop)
    {
        if (isEmpty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepDurationMillis));
            continue;
        }
        Task task = pop();
        for (int y = task.y0; y < task.y1 ; ++y)
        {
            for (int x = task.x0; x < task.x1; ++x)
            {
                m_sampler->samplePixel(x, y);
            }
        }
        task.sampleCount++;
        if (task.sampleCount < task.sampleTarget)
        {
            push(task);
        }
    }
}

unsigned int Threadpool::getNumberOfActiveThreads() const
{
    return m_threads.size();
}

void Threadpool::killThreads()
{
    m_stop = true;
    for (auto& thread : m_threads)
    {
        thread.join();
    }
}