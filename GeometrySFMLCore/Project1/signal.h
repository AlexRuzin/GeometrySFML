#pragma once

#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#include <mutex>
#include <condition_variable>

class EventSignal
{
private:
    bool                            isSignaled;
    std::mutex                      signaledMutex;
    std::condition_variable         signaledCondition;

public:
    EventSignal(void) :
        isSignaled(false)
    {

    }
    ~EventSignal(void)
    {

    }

    bool IsSignaled(void) const
    {
        return isSignaled;
    }
    void SetSignal(void);
    void ResetSignal(void);
    void WaitForSignal(void);
    bool WaitForSignalTimeout(uint32_t timeoutMs);
};

#endif _SIGNAL_H_