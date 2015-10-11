#ifndef __ConsumerThread_h__
#define __ConsumerThread_h__

#include <pthread.h>
#include "thread.h"
#include "wqueue.h"
#include "workItem.h"
class Patient_Lib;
class ConsumerThread : public Thread
{
    friend class Patient_Lib;
    wqueue<WorkItem*>& m_queue;

  public:
    ConsumerThread(wqueue<WorkItem*>& queue) : m_queue(queue) {}

    void* run() {
        // Remove 1 item at a time and process it. Blocks if no items are
        // available to process.
        for (int i = 0;; i++) {
            printf("thread %lu, loop %d - waiting for item...\n",
                  (long unsigned int)self(), i);
            WorkItem* item = (WorkItem*)m_queue.remove();
            printf("thread %lu, loop %d - got one item\n",
                  (long unsigned int)self(), i);
            Patient_Lib::addEntry(item->get());
            delete item;
        }
        return NULL;
    }
};
#endif
