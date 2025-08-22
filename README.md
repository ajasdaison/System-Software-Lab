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
|   |-- fcfs.c
|   |-- priority.c
|   |-- rr.c
|   |-- sjf.c
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
    ```bash
    git clone https://github.com/ajasdaison/System-Software-Lab.git
    cd System-Software-Lab
    ```
2. Create a build directory and configure with CMake:
    ```bash
    mkdir build && cd build
    cmake ..
    ```
3. Build the experiments:
    ```bash
    make
    ```
4. Run a specific experiment (example for exp1):
    ```bash
    ./exp1/exp1_executable
    ```

## 📜 Experiments Overview
The experiments included in this repository cover topics such as:
