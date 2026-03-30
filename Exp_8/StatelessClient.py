import socket

def stateless_request(command_string):
    # We open a NEW connection for every single request
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(('localhost', 12346))
    
    client.send(command_string.encode())
    response = client.recv(1024).decode()
    
    print(f"Request: {command_string}")
    print(f"Server Response: {response}\n")
    client.close()

if __name__ == "__main__":
    # Every request MUST include the filename
    stateless_request("WRITE test.txt Hello_World")
    stateless_request("READ test.txt")