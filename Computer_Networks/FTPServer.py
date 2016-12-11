import socket
import threading
import SocketServer
import os,sys,shutil

Authenticated_List = []
HOST = "0.0.0.0"
AUTH_PORT = int(sys.argv[1])
BUFFER_SIZE = 1024
BASE_PATH = os.path.abspath("/home/bhavya/bhavya/ICT/Projects/Computer_Networks")

class ThreadedTCPRequestHandler(SocketServer.BaseRequestHandler):

    def handle(self):
        data = self.request.recv(1024)
        cur_thread = threading.current_thread()
        response = "{}: {}".format(cur_thread.name, data)
        print data
        usrname,password = data.strip().split(' ')
        Auth_chk_resp,usr_path = self.Authenticate(usrname,password)
        #self.request.sendall(Auth_chk_resp)
        self.usr_base_dir = usr_path

        if(Auth_chk_resp == "OK"): #initiate a passive FTP-TCP connection i.e. from Client to Server
            self.request.send("OK")
            self.curr_dir = self.usr_base_dir
            while True:
                print "--------------------------------------------------------------------------------"
                command = self.request.recv(1024)
                if not command:
                    break
                print "Command: %s" %(command)
                if(command == "Download" or command == "Upload"):
                    data_sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
                    data_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
                    data_sock.bind((HOST, 0))

                    ip,data_port = data_sock.getsockname()

                    data_sock.listen(5)
                    print "\nWaiting for incoming DATA connections..."
                    self.request.send(str(data_port))
                    (data_conn, (ip,port)) = data_sock.accept()
                    self.data_transfer(data_conn,command)

                elif(command == "List"):
                    list_content = self.List()
                    self.request.send(str(list_content))
                elif(command == "chdir"):
                    chdir_cmnd = self.request.recv(1024)
                    if chdir_cmnd == "..":
                        if self.curr_dir == self.usr_base_dir:
                            self.request.send("ACCESS ERROR: DIR ALREADY A BASE DIR")
                        else:
                            self.curr_dir = os.path.split(self.curr_dir)[0]
			    print "CURR DIR:",self.curr_dir
                            self.request.send("CHDIR OK")
                    else:
                        chdir_path = os.path.abspath(os.path.join(self.curr_dir,chdir_cmnd))
			print "CURR DIR:",chdir_path
                        if(os.path.isdir(chdir_path)):
                            self.curr_dir = os.path.abspath(chdir_path)
                            self.request.send("CHDIR OK")
                        else:
                            self.request.send("NAME ERROR: BAD DIRECTORY")
                elif(command == "mkdir"):
                    mkdir_dirname = self.request.recv(1024)
                    mkdirpath = os.path.join(self.curr_dir,mkdir_dirname)
                    if os.path.isdir(mkdirpath):
                        self.request.send("DUPLICATE ERROR: DIR ALREADY EXISTS")
                    else:
                        os.mkdir(mkdirpath)
                        self.request.send("MKDIR OK")
                elif command == "rm":
                    del_file_name = self.request.recv(1024)
                    del_file_path = os.path.join(self.curr_dir,del_file_name)
                    if os.path.isfile(del_file_path):
                        self.request.send("DEL FILE OK")
                        os.remove(del_file_path)
                    else:
                        self.request.send("ERROR: INVALID PATH FOR FILE")
                elif command == "rmdir":
                    del_dir_name = self.request.recv(1024)
                    del_dir_path = os.path.join(self.curr_dir,del_dir_name)
                    if os.path.isdir(del_dir_path):
                        try:
                            os.rmdir(del_dir_path)
                            self.request.send("DEL DIR OK")
                        except OSError as ex:
                            self.request.send("ERROR: DIR NOT EMPTY")
                    else:
                        self.request.send("ERROR: INVALID PATH FOR DIR")
                elif command == "mv":
                    from_name,to_name = self.request.recv(1024).strip().split(' ')
                    from_path = os.path.join(self.curr_dir,from_name)
                    if os.path.exists(from_path):
                        shutil.move(os.path.join(self.curr_dir,from_name),os.path.join(self.curr_dir,to_name))
                        self.request.send("MOVE OK")
                    else:
                        self.request.send("ERROR: PATH DOES NOT EXIST")
                elif(command == "Exit"):
                    break
        else:
            self.request.send(Auth_chk_resp)
        self.request.close()

    def data_transfer(self,data_conn,cmnd):

        if cmnd == "Download": # Client will Download
            dwnld_source = self.request.recv(1024)
            filename = os.path.join(self.curr_dir,dwnld_source)
            if(os.path.isfile(filename)):
                self.request.send("FILE OK")
                fo = open(filename,"r")
                file_size = os.path.getsize(filename)
                self.request.send(str(file_size))
                uploaded = 0
                while True:
                    l = fo.read(1024)
                    while (l):
                        data_conn.send(l)
                        l = fo.read(1024)
                    fo.close()
                    print ""
                    break
            else:
                self.request.send("ERROR: INVALID PATH IN GROUP")

        elif cmnd == "Upload": # Client will Upload, Server will Download
            ack = self.request.recv(1024)
            if(ack=="SENDING OK"):
                upld_target = self.request.recv(1024)
                filename = os.path.join(self.curr_dir,upld_target)

                with open(filename,'wb') as f:
                    print 'file opened'
                    while True:
                        #print('receiving data...')
                        data = data_conn.recv(BUFFER_SIZE)
                        if not data:
                            f.close()
                            data_conn.close()
                            print 'file close()'
                            break
                        # write data to a file
                        f.write(data)
            else:
                print ack
        data_conn.close()

    def Authenticate(self,usrname,password):
        flag = False
        auth_path = os.path.join(BASE_PATH,"Authentication","AUTH_DOC")
        with open(auth_path,"r") as f:
            for i in f:
                usr,passwd,path = i.strip().split(' ')
                if usr == usrname:
                    if passwd == password:
                        flag = True
			break
                    else:
                        return "ERROR: INCORRECT PASSWORD",""
	if flag == True:
	    return "OK",path        
	else:
            return "ERROR: INCORRECT USER NAME",""

    def List(self):
        list_dirs = "\nLISTING FOR %s\n" %(self.curr_dir)
        list_files = ""
        for path,dirs,files in os.walk(self.curr_dir):
            if dirs is not []:
                for i in dirs:
                    list_dirs += "\n" + i + "/"
            if files is not []:
                for i in files:
                    list_files += i + "\n"
            break
        # print list_dirs + list_files
        return list_dirs + "\n" + list_files

class ThreadedTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer):
    pass

def client(ip, port, message):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((ip, port))
    try:
        sock.sendall(message)
        response = sock.recv(1024)
        print "Received: {}".format(response)
    finally:
        sock.close()

if __name__ == "__main__":
    # Port 0 means to select an arbitrary unused port
    # HOST, PORT = "localhost", 0

    server = ThreadedTCPServer((HOST, AUTH_PORT), ThreadedTCPRequestHandler)
    ip, port = server.server_address

    # Start a thread with the server -- that thread will then start one
    # more thread for each request
    server_thread = threading.Thread(target=server.serve_forever)
    # Exit the server thread when the main thread terminates
    server_thread.daemon = True
    server_thread.start()
    print "Server loop running in thread:", server_thread.name

    while True:
        pass
    server.shutdown()
    server.server_close()
