import socket

def stateful_session():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(('localhost', 12345))
    
    # Step 1: Tell the server which file we want (State is created)
    print("Sending: OPEN test.txt")
    client.send("OPEN test.txt".encode())
    print(f"Server: {client.recv(1024).decode()}\n")

    # Step 2: Now we can just say READ. 
    # We don't need to say "test.txt" anymore! The server remembers.
    print("Sending: READ")
    client.send("READ".encode())
    print(f"Server Content: {client.recv(1024).decode()}\n")

    # Step 3: Close the session
    client.send("CLOSE".encode())
    client.close()

if __name__ == "__main__":
    stateful_session()