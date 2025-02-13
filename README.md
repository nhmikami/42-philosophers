# 42-philosophers

The dining philosophers problem is a classical problem in computer science related to deadlock prevention and resource allocation in multi-threaded or multi-process environments.

The problem depicts a scenario where a group of philosophers sit around a circular table, with a fork placed between each pair. The philosophers alternate between eating, sleeping and thinking. But to eat, a philosopher must pick up both the fork to their left and the fork to their right simultaneously.

This problem illustrates key challenges in concurrent programming:
* Deadlocks: a situation where all philosophers pick up one fork and wait indefinitely for the second one, leading to a system-wide halt
* Starvation: a philosopher may never get a chance to eat if others keep taking the forks first
* Race conditions: multiple threads or processes accessing shared resources without proper synchronization, leading to unpredictable results

## Description

The program takes the following arguments:
```sh
./philo num_philos time_to_die time_to_eat time_to_sleep [times_must_eat]
```
* `num_philos`: number of philosophers (and also the number of forks) 
* `time_to_die`: time (in milliseconds) a philosopher can go without eating before dying
* `time_to_eat`: time (in milliseconds) a philosopher takes to eat
* `time_to_sleep`: time (in milliseconds) a philosopher takes to sleep
* `times_must_eat`: number of times each philosopher must eat

The simulation ends when a philosopher dies of starvation, or when all philosophers have finished eating a given number of meals.

The program displays a log of every state change of the philosophers:
```sh
<timestamp_in_ms> <ID> has taken a fork
<timestamp_in_ms> <ID> is eating
<timestamp_in_ms> <ID> is sleeping
<timestamp_in_ms> <ID> is thinking
<timestamp_in_ms> <ID> died
```

### Mandatory
In the mandatory part, each philosopher should be a thread, and each fork should be protected by a mutex.

* A **thread** is a lightweight unit of execution that runs within a process. All threads within a process share the same memory space and resources, making them efficient but requiring synchronization mechanisms to avoid conflicts when accessing shared resources.

* A **mutex** (short for mutual exclusion) is a locking mechanism that provides exclusive access to a shared resource. It allows only one thread to access a critical section of the code at a time, preventing race conditions and maintaining data integrity.

### Bonus
In the bonus part, each philosopher should be a process, and the number of available forks is represented by a counting semaphore since all forks are placed in the middle of the table. Because each process has its own isolated memory space, semaphores are used to coordinate access to the forks.

* A **process** is an independent execution unit with its own memory space and resources. Each process runs in isolation from other processes, requiring additional mechanisms for inter-process communication (IPC).

* A **semaphore** is a synchronization mechanism used to control access to shared resources. Semaphores maintain a count, which represents the number of available resources.
  * Binary semaphore: can be 0 (locked) or 1 (unlocked)
  * Counting semaphore: can have values greater than 1, allowing multiple processes to access a resource simultaneously
