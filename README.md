# futex-investigation

## 1. std::condition_variable
On WSL wait_for and wait_until use futex with : FUTEX_WAIT_BITSET|FUTEX_CLOCK_REALTIME.  
`It may default to CLOCK_REALTIME for compatibility with Windows’ timekeeping.`  
inside container wait_for and wait_until use futex with : FUTEX_WAIT_PRIVATE  
`Native Linux prefers monotonic time for stability.`

## 2. CLOCKS

* CLOCK_REALTIME - adjustment is possible
* CLOCK_MONOTONIC - non adjustable clock
* CLOCK_BOOTTIME - clock that continues to run even during system suspension

## 3. Back then condition variable used to use only CLOCK_REALTIME
[std::condition_variable::wait_for prone to early/late timeout with libstdc++](https://randombitsofuselessinformation.blogspot.com/2018/06/its-about-time-monotonic-time.html)  
[std::condition_variable will work correctly with std::chrono::steady_clock in GCC 10!](https://randombitsofuselessinformation.blogspot.com/2019/09/stdconditionvariable-will-work.html)

https://linux.die.net/man/3/pthread_cond_timedwait
pthread_cond_clockwait

## 4. TODO
* Take a look at semaphore with try_acquire_for/ try_acquire_until
