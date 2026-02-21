import socket
import threading
import time

LB_HOST = "0.0.0.0"
LB_PORT = 8001

# ⚠️ 여기를 네 VM IP에 맞게 바꿔줘
BACKENDS = [
    ("192.168.90.132", 9001, "backend1"),  # VM3 IP
    ("192.168.90.133", 9002, "backend2"),  # VM4 IP
]

_rr_index = 0
_lock = threading.Lock()
_dispatch_counts = {name: 0 for (_, _, name) in BACKENDS}
_total_requests = 0


def choose_backend():
    """간단한 라운드로빈 선택"""
    global _rr_index
    with _lock:
        host, port, name = BACKENDS[_rr_index]
        _rr_index = (_rr_index + 1) % len(BACKENDS)
    return host, port, name


def handle_client(client_conn: socket.socket, client_addr):
    global _total_requests

    backend_host, backend_port, backend_name = choose_backend()
    start_time = time.time()

    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as be_sock:
            be_sock.connect((backend_host, backend_port))

            data = client_conn.recv(1024)
            if not data:
                return

            be_sock.sendall(data)
            resp = be_sock.recv(1024)

            client_conn.sendall(resp)
    except Exception as e:
        print(f"[LB] ERROR while forwarding from {client_addr} to {backend_name}: {e}")
    finally:
        client_conn.close()
        elapsed_ms = (time.time() - start_time) * 1000

        with _lock:
            _dispatch_counts[backend_name] += 1
            _total_requests += 1
            my_count = _dispatch_counts[backend_name]
            total = _total_requests

        # 요청별 분배 상황 찍기
        print(
            f"[LB] req#{total} -> {backend_name}({backend_host}:{backend_port}), "
            f"backend_count={my_count}, latency={elapsed_ms:.2f}ms"
        )

        # 10개 단위로 summary 출력
        if total % 10 == 0:
            with _lock:
                summary = " / ".join(
                    f"{name}={cnt}" for name, cnt in _dispatch_counts.items()
                )
            print(f"[LB] SUMMARY after {total} reqs: {summary}")


def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        s.bind((LB_HOST, LB_PORT))
        s.listen()
        print(f"[LB] listening on {LB_HOST}:{LB_PORT}")

        while True:
            conn, addr = s.accept()
            t = threading.Thread(target=handle_client, args=(conn, addr), daemon=True)
            t.start()


if __name__ == "__main__":
    main()