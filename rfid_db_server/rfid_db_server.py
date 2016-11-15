from socket import *
from threading import *
import sys

#CONFIG
port = 5555
host = ''
s = socket(AF_INET, SOCK_STREAM)
DATABASE_PATH = 'uid_database'
database_lock = Lock()

def on_connect(ip):
    print(ip + ' CONNECT')

def on_disconnect(ip):
    print(ip + ' DISCONNECT')

def database_contains(uid):
    print('CHECKING UID: "' + uid + '"')
    
    with open(DATABASE_PATH, 'r') as database:
        for record in database:
            record = record.strip('\n')
            if record == uid:
                return True
    
    return False

def manage_response(is_positive):
    response = ''
    if is_positive:
       response = 'OK'
    else:
       response = 'NO'
    
    print('RESPONSE MSG: ' + response)
    return response.encode()
    

def client_thred(conn, ip):
   on_connect(ip)
   while True:
       data = conn.recv(4096)
       if not data:
           on_disconnect(ip)
           break

       data = data.decode('utf-8')
       
       database_lock.acquire()   
       response = manage_response(database_contains(data))
       database_lock.release()       

       conn.send(response)
       

def server_loop():
    print('SERVER START')
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
