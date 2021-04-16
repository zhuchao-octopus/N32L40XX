#!/usr/bin/python

from __future__ import print_function
import sys
import os
import getopt
import struct

def usage():
	print ("Usage:	paddingfile [OPTION] srcfile")
	print ("	option:-h, --help		help")
	print ("	       -s, --size		output file size")
	print ("	       -m, --sign		sign")
	print ("	       -o, --output=xx	output file")
	print ("Examples:")
	print ("	./paddingfile.py -s 512 -o ./b.bin a.bin")

def copyfile(src, dest):
    with open(src, "rb") as fsrc:
        with open(dest, "wb") as fdest:
            while True:
                buf = fsrc.read(1024)
                if buf == b"":
                    break
                fdest.write(buf)
    fsrc.close();
    fdest.close();
    return os.path.getsize(src) == os.path.getsize(dest)

def append_padding(fn, padding, size):
	remain = 0
	desired = os.path.getsize(fn) + size
	remain = size
	fp = open(fn, 'ab+')
	while remain > 0:
		s = struct.pack('B', padding)
		fp.write(s)
		remain = remain - 1
	fp.close()
	return desired == os.path.getsize(fn)

def append_sign(fn, sign):
	sign_i = int(sign, 16)
	b = sign_i.to_bytes(length = 2, byteorder='big', signed=False)
	desired = os.path.getsize(fn) + 2
	fp = open(fn, 'ab+')
	fp.write(b)
	fp.close()
	return desired == os.path.getsize(fn)

def append_checksum(fn):
	count = 0
	sum = 0x00
	fsrc = open(fn, "rb")
	while True:
		count = count + 1
		buf = fsrc.read(1)
		if buf == b"":
			break
		if count == os.path.getsize(fn) - 2:
			break
		sum = sum + (buf[0])
	fsrc.close()
	bl = struct.pack('B', sum & 0xff)
	bh = struct.pack('B', (sum >> 8) & 0xff)
	print('sum=%d,%#x,%#x,%#x'% (sum, sum, ord(bl), ord(bh)))

	desired = os.path.getsize(fn) + 2
	fp = open(fn, 'ab+')	
	fp.write(bl)
	fp.write(bh)
	fp.close()
	return desired == os.path.getsize(fn)

def main(argv):
	src_fn = None
	src_fn_size = 0
	dest_fn = None
	dest_size = 0
	sign_str = None
	argc = len(argv)
	
	try:
		opts, args = getopt.getopt(argv, "s:o:m:", ["output size=", "output file=", "sign="])
	except e:
		print ('\033[5;33;44m %s \033[0m' % str(e))
		usage()
		sys.exit(2)

	for opt, arg in opts:
		if opt in ("-h", "--help"):
			usage()
			sys.exit()
		elif opt in ("-s", "--size"):
			dest_size = arg
		elif opt in ("-m", "--sign"):
			sign_str = arg
		elif opt in ("-o", "--output"):
			dest_fn = arg

	if len(args) > 0:
		src_fn = args[0]
		if os.path.exists(src_fn) == False:
			print ('\033[5;33;44m %s not exist! \033[0m' % src_fn)
			sys.exit(3)
		else:
			src_fn_size = os.path.getsize(src_fn)
			if src_fn_size > int(dest_size):
				print ('\033[5;33;44m %s size %d must less then %d! \033[0m' % (src_fn, src_fn_size, dest_size))
				sys.exit(4)
	else:
		print ('\033[5;33;44m Missing path parameter!!! \033[0m')
		usage()
		sys.exit(3)

	print (" src file: %s\n src size: %d\ndest size: %s\ndest file: %s" % (src_fn, src_fn_size, dest_size, dest_fn))

	if copyfile(src_fn, dest_fn):
		print ("\n>>>copy %s to %s ok!" % (src_fn, dest_fn))
	else:
		print ("\n>>>copy %s to %s failed!" % (src_fn, dest_fn))
		sys.exit(4)

	padding = 0x00
	if append_padding(dest_fn, padding, int(dest_size) - os.path.getsize(dest_fn)):
		print ("\n>>>append padding ok!")
	else:
		print ("\n>>>append padding failed!")
		sys.exit(5)

	if append_sign(dest_fn, sign_str):
		print ("\n>>>append sign ok!")
	else:
		print ("\n>>>append sign failed!")
		sys.exit(6)

	if append_checksum(dest_fn):
		print ("\n>>>append checksum ok!")
		sys.exit(0)
	else:
		print ("\n>>>append checksum failed!")
		sys.exit(6)

if __name__ == "__main__":
	if len(sys.argv) > 1:
		main(sys.argv[1:])
	else:
		usage()
		sys.exit(1)