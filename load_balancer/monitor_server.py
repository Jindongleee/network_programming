# monitor_backends_live.py
import socket
import threading
import sys
import time
import os

HOST = "0.0.0.0"
PORT = 9000

# backend IP별 마지막 수신 정보 저장
last_status = {}
lock = threading.Lock()


def handle_client(conn, addr):
    ip = addr[0]

    with conn, conn.makefile("r") as f:
        for line in f:
            msg = line.strip()
            with lock:
                last_status[ip] = msg
    # 연결이 끊겨도 dict는 유지 (최근 값 그대로 보이게)


def print_loop():
    while True:
        # 화면 전체 지우고 커서 맨 위(left-top)로
        sys.stdout.write("\x1b[2J\x1b[H")
        sys.stdout.flush()

        print("[MONITOR] Live view (backend1 & backend2)\n")

        with lock:
            if not last_status:
                print("(waiting for backends...)")
            else:
                for ip in sorted(last_status.keys()):
                    print(f"{ip}: {last_status[ip]}")

        sys.stdout.flush()
        time.sleep(0.5)


def main():
    print("[MONITOR] Live view started.\n")

    # 출력 담당 스레드
    threading.Thread(target=print_loop, daemon=True).start()

    # 서버 오픈
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((HOST, PORT))
        s.listen()

        while True:
            conn, addr = s.accept()
            threading.Thread(target=handle_client, args=(conn, addr), daemon=True).start()


if __name__ == "__main__":
    main()