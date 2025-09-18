import os, socket, threading, time
from contextlib import closing
from app import app

BANNER = "OS Fault Simulator Launcher"

def find_free_port(preferred=5000):
    for p in (preferred, 5080, 5055, 0):
        with closing(socket.socket(socket.AF_INET, socket.SOCK_STREAM)) as s:
            try:
                s.bind(('127.0.0.1', p))
                return s.getsockname()[1]
            except OSError:
                continue
    raise RuntimeError("No free port found")

def main():
    port = find_free_port()
    print(f"[{BANNER}] Starting on http://127.0.0.1:{port}")
    print("[Info] Debug reloader disabled for stability.")
    print("[Info] Press CTRL+C to stop.")
    try:
        app.run(host='127.0.0.1', port=port, debug=False, use_reloader=False)
    except Exception as e:
        print("[FATAL] Server crashed:", e)
        raise

if __name__ == '__main__':
    main()
