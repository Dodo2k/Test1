from http.server import BaseHTTPRequestHandler, HTTPServer
import chilkat
import sys
import time

hostName = "192.168.86.98"  #enter IP of localhost
serverPort = 8080

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "application/xml")
        self.end_headers()
        pa = open("pa2.xml", "r" )   #Enter either the fullpath of the xml file or relocate the xml file to this directory.
        for line in pa:
            self.wfile.write(bytes( line, "utf-8") )

##### signature validation code #####

sbXml = chilkat.CkStringBuilder()
success = sbXml.LoadFile("pa2.xml" , "utf-8")

verifier = chilkat.CkXmlDSig()
success = verifier.LoadSignatureSb(sbXml)
if (success != True):
    print(verifier.lastErrorText())
    sys.exit()

bVerified = verifier.VerifySignature(True)
if (bVerified != True):
    print(verifier.lastErrorText())
    sys.exit()
print("Signature verified!")

##### Webserver initialisation and startup #####
webServer = HTTPServer((hostName, serverPort), MyServer)
print("Server started http://%s:%s" % (hostName, serverPort))

try:
    webServer.serve_forever()
except KeyboardInterrupt:
    pass

webServer.server_close()
print("Server stopped.")
