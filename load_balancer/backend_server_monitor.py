# backend_server_monitor.py
import socket
import threading
import datetime
import time
import os

# ==== 로드밸런서가 붙는 backend 서버 설정 ====
BACKEND_HOST = "0.0.0.0"
BACKEND_PORT = 9002   # LB가 여기로 포워딩

# ==== 모니터링 전송 대상 (VM1) ====
MONITOR_HOST = "192.168.90.135"  # VM1 IP로 바꾸기
MONITOR_PORT = 9000
MONITOR_INTERVAL = 1.0          # 초

# ---------- CPU / MEM 측정 함수 (/proc 기반) ----------

_prev_total = None
_prev_idle = None

def get_cpu_percent_from_proc() -> float:
    """
    /proc/stat을 이용해서 최근 구간의 CPU 사용률 계산
    """
    global _prev_total, _prev_idle

    try:
        with open("/proc/stat", "r") as f:
            fields = f.readline().strip().split()
        # fields 예: ["cpu", user, nice, system, idle, iowait, irq, softirq, steal, ...]
        if fields[0] != "cpu":
            return 0.0

        nums = list(map(int, fields[1:]))

        user, nice, system, idle, iowait, irq, softirq, steal = nums[:8]
        idle_all = idle + iowait
        non_idle = user + nice + system + irq + softirq + steal
        total = idle_all + non_idle

        if _prev_total is None:
            _prev_total, _prev_idle = total, idle_all
            return 0.0

        diff_total = total - _prev_total
        diff_idle = idle_all - _prev_idle

        _prev_total, _prev_idle = total, idle_all

        if diff_total <= 0:
            return 0.0

        cpu_percentage = (1.0 - (diff_idle / diff_total)) * 100.0
        return cpu_percentage
    except Exception:
        return 0.0


def get_mem_percent_from_proc() -> float:
    """
    /proc/meminfo에서 메모리 사용률 계산
    """
    mem_total = None
    mem_available = None

    try:
        with open("/proc/meminfo", "r") as f:
            for line in f:
                if line.startswith("MemTotal:"):
                    mem_total = int(line.split()[1])  # kB
                elif line.startswith("MemAvailable:"):
                    mem_available = int(line.split()[1])  # kB
                if mem_total is not None and mem_available is not None:
                    break

        if mem_total is None or mem_available is None:
            return 0.0

        used = mem_total - mem_available
        return used / mem_total * 100.0
    except Exception:
        return 0.0

# ---------- 실제 backend 서버 (LB가 트래픽 포워딩) ----------

def backend_server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((BACKEND_HOST, BACKEND_PORT))
        s.listen()
        print(f"[BACKEND1] Listening on {BACKEND_HOST}:{BACKEND_PORT} ...")

        while True:
            conn, addr = s.accept()
            threading.Thread(target=handle_client, args=(conn, addr), daemon=True).start()

def heavy_cpu_work(duration: float = 2.0):
    """
    duration 초 동안 CPU를 바쁘게 만드는 작업
    duration을 1~3초 정도로 조정해보면 그래프가 확 튀는 걸 볼 수 있음
    """
    end = time.time() + duration
    x = 0
    while time.time() < end:
        x += 1  # 의미 없는 연산 (CPU만 태움)
    return x

def handle_client(conn, addr):
    try:
        data = conn.recv(1024)
        if not data:
            return

        msg = data.decode().strip()
        # print(f"[BACKEND1] got: {msg} from {addr}")

        # 2) CPU 부하 걸기 (여기서 실제로 CPU를 태움)
        heavy_cpu_work(duration=2.0)  # 필요하면 3.0, 5.0 등으로 더 키워도 됨

        # 3) 정상 응답 반환
        resp = b"PONG from backend1"
        conn.sendall(resp)

    except Exception as e:
        print(f"[BACKEND1 ERROR] {e}")
    finally:
        conn.close()

# ---------- 모니터링 정보를 VM1으로 계속 보내는 스레드 ----------

def monitor_sender():
    # 초기 한 번 호출해서 기준값 세팅
    _ = get_cpu_percent_from_proc()
    time.sleep(MONITOR_INTERVAL)

    while True:
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.connect((MONITOR_HOST, MONITOR_PORT))
                print(f"[MONITOR-SENDER] Connected to monitor {MONITOR_HOST}:{MONITOR_PORT}")

                while True:
                    ts = datetime.datetime.now().isoformat()
                    cpu = get_cpu_percent_from_proc()
                    mem = get_mem_percent_from_proc()

                    line = f"{ts},cpu={cpu:.1f}%,mem={mem:.1f}%\n"
                    s.sendall(line.encode())

                    time.sleep(MONITOR_INTERVAL)

        except Exception as e:
            print(f"[MONITOR-SENDER] Error: {e}, retry in 3 sec...")
            time.sleep(3)

def main():
    # backend 서버 스레드
    t_server = threading.Thread(target=backend_server, daemon=True)
    t_server.start()

    # 모니터링 전송 스레드
    t_monitor = threading.Thread(target=monitor_sender, daemon=True)
    t_monitor.start()

    # 메인 스레드는 그냥 대기
    while True:
        time.sleep(10)

if __name__ == "__main__":
    main()