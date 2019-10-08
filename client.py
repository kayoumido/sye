#!/usr/bin/env python3
import socket
import sys
import os
import subprocess
import struct

REPO_PATH = os.getenv("REPO_PATH")

if REPO_PATH == None:
    REPO_PATH = subprocess.check_output("pwd", shell=True).rstrip()

HOST, PORT = "reds-codecheck-labs.heig-vd.ch", 51003
BUF_SIZE = 1000000 # 1 MB

ARCHIVE_PATH = "rendu_lab03.tar.gz.gpg"

# This is the only way I find of ignoring Eclipse's metadata
CMD_DIFF = "git diff"
CMD_DIFF_FIND = "`find . -type f -not -path '*/.*/*' -not -name '.*' -not -name 'flash'`"
CMD_BRANCH = "git symbolic-ref --short HEAD"

data = " ".join(sys.argv[1:])

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

branch_name = None

def recvall(count):
    buf = b''
    while count:
        newbuf = sock.recv(count)
        if not newbuf: return None
        buf += newbuf
        count -= len(newbuf)

    return buf

def send_branch_name():
    global repo
    global branch_name

    branch_name = subprocess.check_output(CMD_BRANCH, shell=True).rstrip()

    length = len(branch_name)
    sock.sendall(struct.pack('!I', length))
    sock.sendall(branch_name)

def create_patch():
    global branch_name

    patch = subprocess.check_output(CMD_DIFF + " tag_" + branch_name.decode("utf-8") + " " + CMD_DIFF_FIND, shell=True)
    sock.sendall(struct.pack('!I', len(patch)))
    sock.sendall(patch)

def send_patch():
    print("send_patch")

def get_archive():
    try:
        lengthbuf = recvall(4)
        length, = struct.unpack('!I', lengthbuf)
        data = recvall(length)

    except (ConnectionResetError, TypeError) as e:
        print("Connection closed by remote host. " +
               "Are you on the correct branch? (git branch to check) " +
               "Did you submit an empty patch? (git diff to check)")
        print("Also, please check your REPO_PATH variable. " +
              "If the problem persists, please report to your assistants.")
        print(e)
        exit(-1)

    if not data:
        print("Connection closed by remote host. " +
              "Did you submit an empty patch? (git diff to check)")
        print("Also, please check your REPO_PATH variable. " +
              "If the problem persists, please report to your assistants.")
        exit(-1)

    archive = open(ARCHIVE_PATH, "wb")
    archive.write(data)

print("Connecting to " + HOST + ":" + str(PORT))
sock.connect((HOST, PORT))

print("Sending branch name")
send_branch_name()

print("Sending patch")
create_patch()

print("Retrieving archive, this may take some time ...")
get_archive()

print("Signed archive retrieved in " + ARCHIVE_PATH)

sock.close()
