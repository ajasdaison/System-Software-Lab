# System-Software-Lab  

This repository contains **C programming lab experiments** for college coursework.  
The experiments focus on core **System Software concepts** such as process handling,  
file operations, and directory manipulation.  

---

## 📂 Repository Structure
```
System-Software-Lab/
|
|-- src/                        # Contains all C program experiments
|   |-- fcfs.c                  # Experiment 1A: FCFS Scheduling
|   |-- priority.c              # Experiment 1B: Priority Scheduling
|   |-- rr.c                    # Experiment 1C: Shortest Job First
|   |-- sjf.c                   # Experiment 1D: Round Robin
|-- CMakeLists.txt              # CMake Setup
|-- README.md                   # This file
|-- build/                      # Compiled binaries (after build)
```
## ⚙️ Setup & Usage 

### Prerequisites  
- A C compiler (`gcc` or `clang`)  
- [CMake](https://cmake.org/) (for building the experiments)  

### Steps to Build and Run  

1. Clone the repository:  
    ```sh
    git clone https://github.com/ajasdaison/System-Software-Lab.git
    cd System-Software-Lab
    ```
2. Create a build directory and configure with CMake:
    ```sh
    mkdir build && cd build
    cmake ..
    ```
3. Build the experiments:
    ```sh
    make
    ```
4. Run a specific experiment (example for exp1):
    ```sh
    ./exp1/exp1_executable
    ```

## 📜 Experiments Overview
The experiments included in this repository cover topics such as:

### [**🔗 Experiment 1A: FCFS Scheduling**](src/fcfs.c)

Implements the First-Come, First-Served (FCFS) CPU scheduling algorithm—reads process count and burst times, computes waiting and turnaround times for each process, then prints a table and the average waiting/turnaround times.

### [**🔗 Experiment 1B: Priority Scheduling (Non‑preemptive)**](src/priority.c)

Implements Priority scheduling—reads burst times and priorities, sorts by priority (lower = higher), computes waiting and turnaround times, then prints a table and averages.

### [**🔗 Experiment 1C: Shortest Job First (SJF, Non‑preemptive)**](src/sjf.c)

Implements SJF—reads burst times, sorts by shortest burst first, computes waiting and turnaround times, then prints a table and averages.

### [**🔗 Experiment 1B: Round Robin (RR)**](src/rr.c)

Implements Round Robin—reads burst times and a time quantum, simulates cyclic execution to get turnaround and waiting times, then prints a table and averages.

---
