# OS Fault Simulator

An educational Operating System Fault & Recovery simulator featuring:
- Native C terminal application (process management, fault injection, recovery, algorithm demos)
- Python Flask web dashboard (live process visualization, algorithm comparison modal, interactive fault injection & recovery)

## Quick Start Guide

1. Install Prerequisites:
   - Python 3.9 or higher
   - C compiler (GCC/MinGW) if building from source

2. Setup Python Environment:
   ```powershell
   # Create and activate virtual environment (Optional but recommended)
   python -m venv .venv
   .\.venv\Scripts\activate  # On Windows
   source .venv/bin/activate # On Linux/Mac
   
   # Install dependencies
   pip install -r requirements.txt
   ```

3. Run the Project:
   - Option 1: Using pre-compiled executable (Windows)
     ```powershell
     .\simulation.exe
     ```
   - Option 2: Build and run from source
     ```powershell
     gcc -std=c11 -O2 main.c process_manager.c fault_injection.c recovery.c algorithms.c visualization.c ui.c -o simulation
     .\simulation.exe
     ```
   
4. Start the Web Dashboard:
   ```powershell
   python run_server.py
   ```
   
5. Access the Web Interface:
   - Open your browser and go to: http://127.0.0.1:5000

## Features
- Process acquisition (real system snapshot: Windows `tasklist` in C; cross‑platform `psutil` in Python) capped at 10 processes.
- Fault Injection: Deadlock (simulated waiting), CPU overload, Memory thrashing.
- Recovery Strategies: Reset waiting processes, normalize CPU, expand memory allocation.
- Algorithm Demonstrations: Round Robin, Priority Scheduling, simplified Banker's safety check, deadlock cycle scan, FIFO vs LRU page replacement.
- Visualizations: Text Gantt chart, memory map, resource allocation graph (C) + rich web UI (HTML/JS) with charts & logs.

## Repository Layout
```
algorithms.c/.h        # Algorithm demos (scheduling, deadlock, paging)
fault_injection.c/.h   # Fault injection routines
process_manager.c/.h   # Process table acquisition & display
recovery.c/.h          # Detection & recovery logic
ui.c/.h                # Enhanced colored console UI
visualization.c/.h     # Additional text visualizations
main.c                 # C entry point (menu loop)
app.py                 # Flask web server
templates/index.html   # Web dashboard template
os_simulator.html      # Standalone static demo page (legacy prototype)
requirements.txt       # Python dependencies
LICENSE                # MIT license
```

## Prerequisites
### C Simulator
- Windows (for real process fetch via `tasklist`) or other OS (will fall back to dummy data)
- GCC / MinGW / clang supporting C11.

### Web Dashboard
- Python 3.9+
- Packages in `requirements.txt` (`Flask`, `psutil`)

## Build & Run (C Terminal Version)
```powershell
# From repository root
gcc -std=c11 -O2 main.c process_manager.c fault_injection.c recovery.c algorithms.c visualization.c ui.c -o simulation
# Run
./simulation.exe   # or .\simulation.exe on Windows
```
If `tasklist` fails (non‑Windows or permission issue) dummy processes are generated.

## Run Web Dashboard (Flask)
```powershell
# Make sure you've completed the setup steps from Quick Start Guide first
python run_server.py
```
Visit: http://127.0.0.1:5000

Note: Make sure you have activated your Python virtual environment and installed dependencies as shown in the Quick Start Guide.

## REST Endpoints (Summary)
- `GET /api/init` – Initialize process list (real or dummy)
- `GET /api/processes` – Current processes + system status
- `GET /api/logs` – Recent system logs
- `POST /api/inject_fault` – Body: `{ "fault_type": "deadlock|cpu_overload|memory_thrashing" }`
- `POST /api/recover_fault` – Body: `{ "fault_type": "deadlock|cpu_overload|memory_thrashing" }`
- `GET /api/algorithms` – Simulated algorithm performance data
- `GET /api/reset` – Reset to baseline

## Educational Notes & Limitations
- Banker’s algorithm & deadlock detection are illustrative, not production‑grade.
- CPU & memory metrics after fetch are randomized for interactive behavior.
- Faults & recoveries never alter real system processes—only the in‑memory simulation snapshot.
- Page replacement uses fixed synthetic reference string for comparison.

## Safety & Privacy Disclaimer
The simulator only reads a small subset of process metadata (name, PID, CPU %, memory %) at initialization. It never modifies or terminates real processes. All fault states are simulated internally.

## Contributing
Feel free to open issues or PRs for improved algorithms, cross‑platform enhancements, or UI refinements.

## License
MIT – see `LICENSE`.

## Quick Start (Minimal)
```powershell
git clone <your-fork-url>
cd OS Project\OS Project
python -m venv .venv; . .venv/Scripts/Activate.ps1; pip install -r requirements.txt; python app.py
```
Or build the C version as shown above.

## Future Ideas
- True resource allocation matrices for Banker’s algorithm
- More scheduling algorithms (SJF, Multilevel Queue)
- Persistent configuration & logging
- Cross‑platform process enumeration in C (using `psapi` / `procfs`)
- Unit tests for algorithm modules

Enjoy exploring OS concepts!
