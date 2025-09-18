from flask import Flask, render_template, jsonify, request
import json
import random
import time
import threading
from datetime import datetime
import psutil  # For getting real process data

app = Flask(__name__)

# Global variables to store system state
processes = []
system_logs = []
system_status = {
    'cpu_usage': 0,
    'memory_usage': 0,
    'process_count': 0,
    'status': 'ONLINE'
}

def get_real_processes():
    """Get real running processes from the system"""
    real_processes = []
    try:
        for proc in psutil.process_iter(['pid', 'name', 'cpu_percent', 'memory_percent']):
            try:
                pinfo = proc.info
                if pinfo['name'] and pinfo['pid'] > 0:
                    real_processes.append({
                        'pid': pinfo['pid'],
                        'name': pinfo['name'],
                        'cpu': pinfo['cpu_percent'] or random.uniform(0.5, 15.0),
                        'memory': pinfo['memory_percent'] or random.uniform(1.0, 20.0),
                        'state': random.choice(['READY', 'READY', 'READY', 'RUNNING']),
                        'priority': random.randint(1, 5),
                        'burst_time': random.randint(2, 10)
                    })
                    
                    if len(real_processes) >= 10:  # Limit to 10 processes
                        break
            except (psutil.NoSuchProcess, psutil.AccessDenied):
                continue
    except Exception as e:
        print(f"Error getting real processes: {e}")
        # Fallback to dummy data
        real_processes = generate_dummy_processes()
    
    return real_processes

def generate_dummy_processes():
    """Generate dummy process data if real data isn't available"""
    dummy_names = ['chrome.exe', 'vscode.exe', 'discord.exe', 'firefox.exe', 
                   'spotify.exe', 'notepad.exe', 'explorer.exe', 'steam.exe']
    
    dummy_processes = []
    for i, name in enumerate(dummy_names):
        dummy_processes.append({
            'pid': 1000 + i * 100,
            'name': name,
            'cpu': random.uniform(1.0, 20.0),
            'memory': random.uniform(5.0, 25.0),
            'state': 'READY' if i > 1 else 'RUNNING',
            'priority': random.randint(1, 5),
            'burst_time': random.randint(3, 8)
        })
    
    return dummy_processes

def add_log(message, log_type='info'):
    """Add a log entry with timestamp"""
    timestamp = datetime.now().strftime('%H:%M:%S')
    log_entry = {
        'timestamp': timestamp,
        'message': message,
        'type': log_type
    }
    system_logs.insert(0, log_entry)
    
    # Keep only last 50 logs
    if len(system_logs) > 50:
        system_logs.pop()

def update_system_status():
    """Update global system status"""
    if processes:
        avg_cpu = sum(p['cpu'] for p in processes) / len(processes)
        total_memory = sum(p['memory'] for p in processes)
        
        system_status['cpu_usage'] = round(avg_cpu, 1)
        system_status['memory_usage'] = round(total_memory, 1)
        system_status['process_count'] = len(processes)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/init')
def init_system():
    """Initialize the system with real or dummy process data"""
    global processes
    
    add_log('Initializing OS Fault Simulator...', 'info')
    
    # Try to get real processes, fallback to dummy data
    processes = get_real_processes()
    
    add_log(f'Loaded {len(processes)} processes from system', 'success')
    add_log('OS Fault Simulator ready', 'info')
    
    update_system_status()
    
    return jsonify({
        'status': 'success',
        'message': 'System initialized successfully'
    })

@app.route('/api/processes')
def get_processes():
    """Get current process list"""
    update_system_status()
    return jsonify({
        'processes': processes,
        'system_status': system_status
    })

@app.route('/api/logs')
def get_logs():
    """Get system logs"""
    return jsonify({'logs': system_logs[:20]})  # Return last 20 logs

@app.route('/api/inject_fault', methods=['POST'])
def inject_fault():
    """Inject a specific fault into the system"""
    fault_type = request.json.get('fault_type')
    
    if fault_type == 'deadlock':
        # Set two random processes to waiting state
        if len(processes) >= 2:
            indices = random.sample(range(len(processes)), 2)
            for idx in indices:
                processes[idx]['state'] = 'WAITING'
            
            add_log(f'Deadlock injected: Processes {processes[indices[0]]["pid"]} and {processes[indices[1]]["pid"]} are now waiting', 'error')
    
    elif fault_type == 'cpu_overload':
        # Set a random process to 100% CPU usage
        idx = random.randint(0, len(processes) - 1)
        processes[idx]['cpu'] = 100.0
        add_log(f'CPU overload injected: Process {processes[idx]["name"]} consuming 100% CPU', 'warning')
    
    elif fault_type == 'memory_thrashing':
        # Increase memory usage of multiple processes
        affected = 0
        for process in processes:
            if random.random() > 0.6:
                process['memory'] = min(process['memory'] * 2, 95.0)
                affected += 1
        
        add_log(f'Memory thrashing injected: {affected} processes experiencing high memory usage', 'warning')
    
    update_system_status()
    return jsonify({'status': 'success', 'message': f'Fault {fault_type} injected successfully'})

@app.route('/api/recover_fault', methods=['POST'])
def recover_fault():
    """Recover from a specific fault"""
    fault_type = request.json.get('fault_type')
    
    if fault_type == 'deadlock':
        # Reset waiting processes to ready
        recovered = 0
        for process in processes:
            if process['state'] == 'WAITING':
                process['state'] = 'READY'
                recovered += 1
        
        add_log(f'Deadlock resolved: {recovered} processes recovered using resource preemption', 'success')
    
    elif fault_type == 'cpu_overload':
        # Reset high CPU usage processes
        recovered = 0
        for process in processes:
            if process['cpu'] > 90:
                process['cpu'] = random.uniform(5.0, 25.0)
                recovered += 1
        
        add_log(f'CPU overload resolved: {recovered} processes normalized using priority scheduling', 'success')
    
    elif fault_type == 'memory_thrashing':
        # Reduce memory usage to normal levels
        recovered = 0
        for process in processes:
            if process['memory'] > 80:
                process['memory'] = random.uniform(10.0, 35.0)
                recovered += 1
        
        add_log(f'Memory thrashing resolved: {recovered} processes optimized using LRU page replacement', 'success')
    
    update_system_status()
    return jsonify({'status': 'success', 'message': f'Recovery from {fault_type} completed'})

@app.route('/api/algorithms')
def get_algorithms():
    """Get algorithm comparison data"""
    add_log('Generating algorithm comparison analysis...', 'info')
    
    # Simulate algorithm performance data
    algorithms = {
        'scheduling': {
            'round_robin': {
                'name': 'Round Robin (Time Quantum: 3ms)',
                'avg_turnaround': random.randint(10, 15),
                'throughput': random.randint(80, 95),
                'pros': ['Fair allocation', 'Good for interactive processes', 'No starvation'],
                'cons': ['Higher context switching overhead', 'Not optimal for CPU-bound tasks']
            },
            'priority': {
                'name': 'Priority Scheduling',
                'avg_turnaround': random.randint(8, 12),
                'throughput': random.randint(90, 100),
                'pros': ['Efficient for system processes', 'Lower turnaround time', 'Good for real-time systems'],
                'cons': ['Possible starvation', 'Priority inversion problems']
            }
        },
        'memory': {
            'fifo': {
                'name': 'FIFO Page Replacement',
                'page_faults': random.randint(6, 9),
                'efficiency': random.randint(60, 75),
                'pros': ['Simple implementation', 'Low overhead'],
                'cons': ['Poor performance', 'Belady\'s anomaly possible']
            },
            'lru': {
                'name': 'LRU Page Replacement',
                'page_faults': random.randint(3, 6),
                'efficiency': random.randint(80, 95),
                'pros': ['Better performance', 'Good locality of reference'],
                'cons': ['Higher implementation complexity', 'More overhead']
            }
        },
        'deadlock': {
            'bankers': {
                'name': 'Banker\'s Algorithm',
                'safe_state': random.choice([True, False]),
                'detection_time': random.randint(5, 15),
                'pros': ['Prevents deadlock', 'Safe resource allocation'],
                'cons': ['Requires advance knowledge', 'Conservative approach']
            }
        }
    }
    
    add_log('Algorithm comparison analysis completed', 'success')
    return jsonify({'algorithms': algorithms})

@app.route('/api/reset')
def reset_system():
    """Reset the system to initial state"""
    global processes
    
    add_log('Resetting system to initial state...', 'info')
    
    # Reset all processes to normal state
    for i, process in enumerate(processes):
        process['cpu'] = random.uniform(2.0, 15.0)
        process['memory'] = random.uniform(5.0, 25.0)
        process['state'] = 'RUNNING' if i < 2 else 'READY'
    
    update_system_status()
    add_log('System reset completed successfully', 'success')
    
    return jsonify({'status': 'success', 'message': 'System reset completed'})

def simulate_real_time_updates():
    """Background thread to simulate real-time system updates"""
    while True:
        time.sleep(5)  # Update every 5 seconds
        
        # Simulate small changes in CPU usage for running processes
        for process in processes:
            if process['state'] == 'RUNNING' and process['cpu'] < 90:
                change = random.uniform(-2, 2)
                process['cpu'] = max(0.1, min(process['cpu'] + change, 100))
        
        update_system_status()

if __name__ == '__main__':
    # NOTE: For stable launches prefer: python run_server.py
    update_thread = threading.Thread(target=simulate_real_time_updates, daemon=True)
    update_thread.start()
    print("[Launcher] OS Fault Simulator starting (direct mode)")
    print("[Launcher] Visit: http://127.0.0.1:5000")
    # Disable reloader to prevent double start issues
    app.run(debug=False, use_reloader=False, host='127.0.0.1', port=5000)
