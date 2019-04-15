
//! Qt Object interface to rest of UCC work to allow Qt Thread use
/*!
* \file UCCWorker.cpp
*
* This file encapsulates Qt implementation details and dependencies
* so that the rest of UCC is relatively unchanged.
*
* ADDED to UCC 2015_12 release by Randy Maxwell
*   Changes started on 2015_12_03
*   Changes  ended  on 2015_12_05
*   Addition of Multithreading performance enhancement feature
*		Example: UCC -threads 4 (to have 4 worker threads)
*/

#include    "../src/UCCThread.h"

#ifdef  ENABLE_THREADS

#include    <QThread>
#include    "UCCWorker.h"

// See UCCThread.cpp
extern void prv_WorkThread_Function( const unsigned int thr_array_idx );

void WorkerThread::run()
{
    // Implement run to only call UCC
    // to prevent any Event loops from running
    prv_WorkThread_Function( thread_array_idx );
}


// Constructor to call to base class constructor
UCCWorker::UCCWorker()
{}

// Destructor to call to base class destructor
UCCWorker::~UCCWorker()
{}

// Provide a Qt Object wrapper
WorkerThread * UCCWorker::startWorkInAThread( const unsigned int thread_idx )
{
    // Call into the thread procedure.
    // This will run as long as needed.
    WorkerThread *pWorkerThread = new WorkerThread;

    pWorkerThread->thread_array_idx = thread_idx;

    connect( pWorkerThread, SIGNAL( finished() ),
             pWorkerThread, SLOT( deleteLater() ) );

    // Start Qt Thread which will Call UCC worker thread proc
    pWorkerThread->start();

    return pWorkerThread;
}

#endif
