#include "../../include/multi_thread/ring_buffer.h"
#include <stdlib.h>
ring_buffer::ring_buffer (int size) {
    m_size = 1;
    while (m_size < size) {
        m_size = m_size << 1;
    }
    m_buffer = new void*[m_size];
    pthread_mutex_init (&m_dequeue_lock, NULL);
    pthread_mutex_init (&m_enqueue_lock, NULL);
    m_dequeue_ptr = 0;
    m_enqueue_ptr = 0;
}

int ring_buffer::is_empty () {// 获取队列是否为空的快照
    if (m_dequeue_ptr == m_enqueue_ptr) {
        return 1;
    }
    return 0;
}

int ring_buffer::is_full () {// 获取队列是否为满的快照
    if (m_dequeue_ptr == ((m_enqueue_ptr + 1) & (m_size - 1))) {
        return 1;
    }
    return 0;
}

int ring_buffer::get_element (void** data) {
    // 非阻塞获取出队锁
    while (pthread_mutex_trylock (&m_dequeue_lock) != 0);
    if (is_empty ()) {
        pthread_mutex_unlock (&m_dequeue_lock);
        return 0;// 队列为空
    } else {
        (*data) = m_buffer[m_dequeue_ptr];
        m_dequeue_ptr = (m_dequeue_ptr + 1) & (m_size - 1);
        pthread_mutex_unlock (&m_dequeue_lock);
        return 1;// 获取成功
    }
}

int ring_buffer::add_element (void* data) {
    // 非阻塞获取入队锁
    while (pthread_mutex_trylock (&m_enqueue_lock) != 0);
    if (is_full ()) {
        pthread_mutex_unlock (&m_enqueue_lock);
        return 0;// 队列已满
    } else {
        m_buffer[m_enqueue_ptr] = data;
        m_enqueue_ptr = (m_enqueue_ptr + 1) & (m_size - 1);
        pthread_mutex_unlock (&m_enqueue_lock);
        return 1;// 添加成功
    }
}

ring_buffer::~ring_buffer () {
    delete[] m_buffer;
}
