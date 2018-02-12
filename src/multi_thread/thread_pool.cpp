#include "../../include/multi_thread/thread_pool.h"
#include <iostream>
#include <atomic>
using namespace std;

atomic_int m_finish_counter;
/*thread func*/
void *get_task (void *arg) {
    ring_buffer* rb = (ring_buffer*) arg;
    while (1) {
        task* t_task = 0;
        int flag = rb -> get_element ((void**) (&t_task));
        if (flag != 0) {// 获取成功
            t_task -> run ();// 运行task
            ++m_finish_counter;
        }
    }
}

thread_pool::thread_pool () {
    m_worker_num = 4;
    m_task_buffers = new ring_buffer*[m_worker_num];
    for (int i = 0; i < m_worker_num; ++i) {
        m_task_buffers[i] = new ring_buffer (1000);
    }
    m_workers = new pthread_t[m_worker_num];
    for (int i = 0; i < m_worker_num; ++i) {
        pthread_create (&m_workers[i], NULL, get_task, m_task_buffers[i]);
    }
}

thread_pool* thread_pool::instance = new thread_pool ();
thread_pool* thread_pool::get_instance () {
    return instance;
}

void thread_pool::add_job (task* t_task) {
    int idx = m_idx % m_worker_num;
    ring_buffer* m_task_buffer = m_task_buffers[idx];
    while (m_task_buffer -> add_element ((void*) t_task) == 0);
    m_idx = m_idx + 1;
}

void thread_pool::add_job_list (vector<task*> job_list) {
    m_finish_counter = 0;
    for (int i = 0; i < job_list.size (); ++i) {
        add_job (job_list[i]);
    }
    while (m_finish_counter < job_list.size ());
}

thread_pool::~thread_pool () {
    for (int i = 0; i < m_worker_num; ++i) {
        delete m_task_buffers[i];
    }
    delete[] m_task_buffers;
}

void task::run () {
}
