import socket
import threading
import os

class StatelessFileServer:
    def __init__(self, host, port, root_dir):
        self.host = host
        self.port = port
        self.root_dir = root_dir
        if not os.path.exists(root_dir):
            os.makedirs(root_dir)

    def handle_client(self, conn, addr):
        try:
            data = conn.recv(1024).decode()
            if data:
                # Format: "READ <filename>" or "WRITE <filename> <content>"
                parts = data.split(maxsplit=2)
                command = parts[0].upper()
                filename = parts[1]

                if command == "READ":
                    path = os.path.join(self.root_dir, filename)
                    if os.path.exists(path):
                        with open(path, 'r') as f:
                            response = f.read()
                    else:
                        response = "ERROR: File not found."
                
                elif command == "WRITE":
                    content = parts[2]
                    with open(os.path.join(self.root_dir, filename), 'w') as f:
                        f.write(content)
                    response = "SUCCESS: File written."
                else:
                    response = "ERROR: Invalid Stateless Request."
                
                conn.send(response.encode())
        finally:
            conn.close()

    def start(self):
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server.bind((self.host, self.port))
        server.listen()
        print(f"Stateless Server listening on {self.host}:{self.port}")
        while True:
            conn, addr = server.accept()
            # In stateless, connections are often closed immediately after the request
            self.handle_client(conn, addr)

if __name__ == "__main__":
    StatelessFileServer('localhost', 12346, './stateless_files').start()