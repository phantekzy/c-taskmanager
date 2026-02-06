# Linux System Monitor (C-Systems)

A high-efficiency, real-time system telemetry tool developed in C. This utility bypasses traditional overhead by interfacing directly with the Linux Kernel via the `/proc` virtual filesystem, providing a live dashboard of hardware utilization and process lifecycle management.

---

## Technical Architecture

### 1. Kernel Telemetry Logic
Instead of relying on heavy external libraries, this tool performs raw parsing of kernel-exported data:
* **/proc/stat**: Used for global CPU work-unit analysis.
* **/proc/meminfo**: Used for physical memory page statistics.
* **/proc/[PID]/stat**: Used for individual process threading and state metadata.



### 2. Differential CPU Analysis
The monitor calculates CPU load using a differential snapshot method. Since `/proc/stat` provides cumulative "jiffies" (clock ticks) since boot, the program:
1.  Captures a baseline of `work` and `idle` ticks.
2.  Waits for a 1-second interval.
3.  Calculates the delta ($\Delta$): 
    $$\text{Usage \%} = \left( \frac{\Delta \text{Total} - \Delta \text{Idle}}{\Delta \text{Total}} \right) \times 100$$

### 3. Stream Parsing & Memory Efficiency
To maintain a near-zero memory footprint, the application utilizes `fscanf` with **assignment suppression** (`%*`). This allows the "reading head" to skip unnecessary kernel data fields directly in the buffer without allocating RAM for temporary variables.



---

## Data Mapping (Procfs Specification)

The application implements a strict 1-to-1 mapping of the Linux Process Specification to reach field index 20 (Thread Count):

| Position | Specifier | Description | Action |
| :--- | :--- | :--- | :--- |
| 1 | `%*d` | PID | Ignored (extracted from directory) |
| 2 | `(%[^)])` | Command | **Stored** (Regex-style capture) |
| 3 | `%c` | State | **Stored** (R=Run, S=Sleep, D=Wait) |
| 4-19 | `%*d/%*u` | Kernel Metadata | Ignored (sequential skip) |
| 20 | `%d` | Thread Count | **Stored** |

---

## Performance Interface (ANSI/VT100)

The UI utilizes **ANSI Escape Sequences** to create a persistent dashboard. By using `\033[H` (Cursor Home) and `\033[J` (Clear from cursor), the program overwrites the existing terminal buffer. This technique:
* Eliminates screen flickering.
* Reduces I/O system calls by updating only once per cycle.
* Preserves terminal history above the program execution point.



---

## Build and Deployment

### Requirements
* **OS**: Linux (Kernel 2.6.x+)
* **Compiler**: GCC 4.8+ or Clang
* **Standards**: C99 or later

### Compilation
Utilize the `-O3` flag for maximum compiler optimization regarding loop unrolling and branch prediction:
```bash
gcc -O3 main.c -o sys_monitor
