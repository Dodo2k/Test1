'''import socket

host = '127.0.0.1'
port = 65432
with socket.socket(socket.AF_INET , socket.SOCK_STREAM) as s:
    s.connect((host , port ))
    data = "The world shall know peace"
    data = bytes( data , 'utf-8')
    s.sendall(data)
    data = s.recv(1024)
print('Received' , repr(data))'''

import requests as req


fl = open("pa.txt","r")
pa = ""
for line in fl:
    pa += line

params = {
    "Host: ":"192.168.86.90",
    "Connection: ":"close"
}

url = "http://192.168.86.90/data/"
url += "?pa="
url += pa
print("Connecting to ",url)

r=req.get( url , params = None)


