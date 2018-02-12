#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_
#include <pthread.h>
class ring_buffer {
    private:
        int m_dequeue_ptr;
        int m_enqueue_ptr;
        int m_size;
        void** m_buffer;
        pthread_mutex_t m_dequeue_lock;// 多消费者锁
        pthread_mutex_t m_enqueue_lock;// 多生产者锁
    public:
        ring_buffer (int size);
        int is_full ();
        int is_empty ();
        int get_element (void** data);
        int add_element (void* data);
        ~ring_buffer ();
};
#endif
