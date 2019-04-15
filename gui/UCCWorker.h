//! Qt Object interface to rest of UCC work to allow Qt Thread use
/*!
* \file UCCWorker.h
*
* This file encapsulates Qt Thread implementation details and dependencies
* so that the rest of UCC is relatively unchanged.
*
* ADDED to UCC 2015_12 release by Randy Maxwell
*   Changes started on 2015_12_03
*   Changes  ended  on 2015_12_03
*   Addition of Multithreading performance enhancement feature
*		Example: UCC -threads 4 (to have 4 worker threads)
*/

#ifndef UCCWORKER_H
#define UCCWORKER_H

#include    "../src/UCCThread.h"

#ifdef  ENABLE_THREADS

#include    <QObject>
#include    <QThread>

// Declare a class derived from the QThread
class WorkerThread : public QThread {

    // Declare here
    void run();

public:
    unsigned int thread_array_idx;
};

// QT Object
class UCCWorker : public QObject
{

public:
    UCCWorker();
    ~UCCWorker();

    // Interface to do UCC processing
    WorkerThread * startWorkInAThread( const unsigned int thread_idx );
};

#endif  // ENABLE_THREADS

#endif // UCCWORKER_H

