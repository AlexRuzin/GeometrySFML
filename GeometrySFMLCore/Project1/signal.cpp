#include "signal.h"

#include <chrono>
#include <mutex>

void EventSignal::SetSignal(void)
{
    {
        std::lock_guard<std::mutex> lock(signaledMutex);
        isSignaled = true;
    }

    signaledCondition.notify_all();
}

void EventSignal::ResetSignal(void)
{
    isSignaled = false;
}

void EventSignal::WaitForSignal(void)
{
    std::unique_lock<std::mutex> signalLock(signaledMutex);
    signaledCondition.wait(signalLock, [this] { return isSignaled; });
}

bool EventSignal::WaitForSignalTimeout(uint32_t timeout_ms)
{
    std::unique_lock<std::mutex> signalLock(signaledMutex);
    return signaledCondition.wait_for(signalLock, std::chrono::milliseconds(timeout_ms), [this] { return isSignaled; });
}

