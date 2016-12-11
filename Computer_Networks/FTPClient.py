import socket
import getpass
import glob, os, sys


def signin(usrnm,grppass):
    au.send(usrnm+" "+grppass)

def upload(filepath,sock):
    f = open(filepath,'rb')
    file_size = os.path.getsize(filepath)
    uploaded = 0
    l = f.read(BUFFER_SIZE)
    while (l):
        sock.send(l)
        uploaded += len(l)
        #print('Sent ',repr(l))
        print float(100*uploaded/file_size)," percent complete         \r",
        l = f.read(BUFFER_SIZE)
    f.close()
    print "\nUPLOAD COMPLETE"

def download(sock,filename,file_size):
    downloaded = 0
    with open(os.path.join(os.getcwd(),filename), 'wb') as f:
        print '\nFILE OPENED FOR WRITING...'
        while True:
            data = sock.recv(BUFFER_SIZE)
            if not data:
                break
            downloaded += len(data)
            print float(100*downloaded/file_size)," percent complete         \r",
            f.write(data)
    print "\nFILE DOWNLOADED."

def recvall(sock):
    all = ""
    while True:
        data = sock.recv(BUFFER_SIZE)
        if not data:
            break
        all += data
    return all

TCP_IP = str(sys.argv[1])
TCP_PORT_Authentication = int(sys.argv[2])
BUFFER_SIZE = 1024

au = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
au.connect((TCP_IP, TCP_PORT_Authentication))

usrname = raw_input("Username:")
grppassword = getpass.getpass(prompt="Group Password:")
signin(usrname,grppassword)

resp = au.recv(BUFFER_SIZE)

#print data

if(resp=='OK'):
    print "RESPONSE:",resp
    print "CONNECTION SUCCESSFUL!"
    while True:
        #filelist = recvall(s)
        #print filelist
        list_display = """--------------------------------------------------------------------------------
\n1. Download\n2. Upload\n3. List Content\n4. Change Directory\n5. Make Directory\n6. Delete File\n7. Delete Directory
8. Move\n9. Exit\nEnter Choice: """
	try:
	        choice = int(raw_input(list_display))
	except (NameError,EOFError,ValueError):
		choice = 0
        if(choice==1):
            au.send("Download")
            port = au.recv(BUFFER_SIZE)
            print "\nSERVER DATA PORT:",port

            data_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            data_sock.connect((TCP_IP, int(port)))

            filename = raw_input("Enter filename to be downloaded: ")
            au.send(filename)
            cmnd_response = au.recv(1024)
            if(cmnd_response == "FILE OK"):
                file_size = au.recv(1024)
                download(data_sock,filename,int(file_size))
            else:
                print cmnd_response
            data_sock.close()

        elif(choice==2):
            au.send("Upload")
            port = au.recv(BUFFER_SIZE)
            print "\nSERVER DATA PORT:",port
            data_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            data_sock.connect((TCP_IP, int(port)))

            filepath = raw_input("Enter path to be uploaded:")

            if(os.path.isfile(filepath)):
                print "Starting to Upload"
                filename = os.path.split(filepath)[1]
                au.send("SENDING OK")
                au.send(filename)
                #cmnd_response = au.recv(BUFFER_SIZE)
                upload(filepath,data_sock)
            else:
                au.send("CLIENT ERROR")
                print "INVALID PATH! PLEASE TRY AGAIN."
            data_sock.close()

        elif(choice==3):
            au.send("List")
            content_list = au.recv(BUFFER_SIZE)
            print content_list
        elif(choice==4):
            cd_cmnd = raw_input("\ncd ")
            au.send("chdir")
            au.send(cd_cmnd)
            resp = au.recv(1024)
            print resp
        elif(choice==5):
            mkdir_cmnd = raw_input("\nmkdir ")
            au.send("mkdir")
            au.send(mkdir_cmnd)
            resp = au.recv(1024)
            print resp
        elif(choice==6):
            rmfile_cmnd = raw_input("\nrm ")
            au.send("rm")
            au.send(rmfile_cmnd)
            resp = au.recv(1024)
            print resp
        elif(choice==7):
            rmdir_cmnd = raw_input("\nrmdir ")
            au.send("rmdir")
            au.send(rmdir_cmnd)
            resp = au.recv(1024)
            print resp
        elif(choice==8):
            rename_cmnd = raw_input("\nmv ")
            au.send("mv")
            au.send(rename_cmnd)
            resp = au.recv(1024)
            print resp
        elif(choice==9):
            au.send("Exit")
            au.close()
            break
        else:
            print("Please Enter valid Choice!\n")
else:
    print "RESPONSE:",resp
