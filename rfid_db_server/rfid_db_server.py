from socket import *
from threading import *
import sys

#CONFIG
port = 5555
host = ''
s = socket(AF_INET, SOCK_STREAM)

def client_thred(conn, ip):
   print(ip + ' CONNECT')
   while True:
       data = conn.recv(4096)
       if not data:
        print(ip + " DISCONNECT")
        break
       data = data.decode('utf-8')
       print(ip + ' SENT "' + data + '"')
       if data == 'b2ba9d4':
        response = str('OK').encode()
       else:
        response = str('NO').encode()
       conn.send(response)
       

def server_loop():
    while True:
        conn, addr = s.accept()
        Thread(target=client_thred, args=(conn, addr[0]), daemon=True).start()

def main():
    try:
        s.bind((host, port))
    except socket.error as e:
        print(str(e))
    s.listen()
    server_loop()


if __name__ == '__main__':
    main()
