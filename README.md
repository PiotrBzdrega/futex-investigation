# futex-investigation

## 1. std::condition_variable
On WSL wait_for and wait_until use futex with : FUTEX_WAIT_BITSET|FUTEX_CLOCK_REALTIME.  
`It may default to CLOCK_REALTIME for compatibility with Windows’ timekeeping.`  
inside container wait_for and wait_until use futex with : FUTEX_WAIT_PRIVATE  
`Native Linux prefers monotonic time for stability.`

## 2. std::condition_variable_any

## CLOCKS

* CLOCK_REALTIME - adjustment is possible
* CLOCK_MONOTONIC - non adjustable clock
* CLOCK_BOOTTIME - clock that continues to run even during system suspension