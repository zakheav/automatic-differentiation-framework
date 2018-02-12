#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_
#include "ring_buffer.h"
#include <pthread.h>
#include <vector>
class task {
    public:
        virtual void run ();
};

void *get_task (void *arg);
class thread_pool {
    protected:
        thread_pool ();
    private:
        static thread_pool* instance;
        ring_buffer** m_task_buffers;// 每个worker独享一个ring_buffer
        pthread_t* m_workers;
        int m_idx;
    public:
        int m_worker_num;
        static thread_pool* get_instance ();
        void add_job (task* t_task);
        void add_job_list (std::vector<task*> job_list);
        ~thread_pool ();
};
#endif
