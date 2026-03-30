import socket
import threading
import os

class StatefulFileServer:
    def __init__(self, host, port, root_dir):
        self.host = host
        self.port = port
        self.root_dir = root_dir
        self.sessions = {}  # Memory: { 'IP:Port' : 'filename' }
        
        # Ensure the directory exists
        if not os.path.exists(root_dir):
            os.makedirs(root_dir)

    def handle_client(self, conn, addr):
        print(f"[NEW CONNECTION] {addr} connected.")
        client_key = f"{addr[0]}:{addr[1]}"
        
        try:
            while True:
                # Receive data from client
                data = conn.recv(1024).decode()
                if not data: 
                    break # Normal exit if client closes connection
                
                parts = data.split(maxsplit=1)
                command = parts[0].upper()
                response = ""

                # --- STATEFUL LOGIC START ---
                
                if command == "OPEN":
                    if len(parts) > 1:
                        filename = parts[1]
                        self.sessions[client_key] = filename # Server "Remembers"
                        response = f"SUCCESS: {filename} opened in session."
                    else:
                        response = "ERROR: Please specify a filename (e.g., OPEN test.txt)"

                elif command == "WRITE":
                    filename = self.sessions.get(client_key)
                    if filename and len(parts) > 1:
                        content = parts[1]
                        with open(os.path.join(self.root_dir, filename), 'w') as f:
                            f.write(content)
                        response = f"SUCCESS: Content written to {filename}."
                    else:
                        response = "ERROR: Open a file first or provide content."

                elif command == "READ":
                    filename = self.sessions.get(client_key) # Server retrieves from memory
                    if filename:
                        path = os.path.join(self.root_dir, filename)
                        if os.path.exists(path):
                            with open(path, 'r') as f:
                                response = f.read()
                        else:
                            response = f"ERROR: File '{filename}' does not exist on disk."
                    else:
                        response = "ERROR: No file opened in this session. Use OPEN first."

                elif command == "CLOSE":
                    if client_key in self.sessions:
                        del self.sessions[client_key]
                    response = "SUCCESS: Session closed."
                    conn.send(response.encode())
                    break # Exit the loop to disconnect
                
                else:
                    response = "ERROR: Unknown Command. Use OPEN, WRITE, READ, or CLOSE."

                # --- STATEFUL LOGIC END ---

                conn.send(response.encode())
                
        except (ConnectionResetError, ConnectionAbortedError):
            print(f"[DISCONNECTED] Client {addr} abruptly closed the connection.")
        except Exception as e:
            print(f"[ERROR] {addr}: {e}")
        finally:
            # Clean up session memory when client leaves
            if client_key in self.sessions:
                del self.sessions[client_key]
            conn.close()
            print(f"[CONNECTION CLOSED] {addr}")

    def start(self):
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # Allows us to restart the server immediately without "Address already in use" errors
        server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server.bind((self.host, self.port))
        server.listen()
        print(f"Stateful Server listening on {self.host}:{self.port}")
        
        while True:
            conn, addr = server.accept()
            thread = threading.Thread(target=self.handle_client, args=(conn, addr))
            thread.start()

if __name__ == "__main__":
    StatefulFileServer('localhost', 12345, './stateful_files').start()