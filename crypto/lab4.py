#!/usr/bin/env python

import urllib2
import sys

TARGET = 'http://crypto-class.appspot.com/po?er='
#--------------------------------------------------------------
# padding oracle
#--------------------------------------------------------------
class PaddingOracle(object):
	def query(self, i, q):
		target = TARGET + urllib2.quote("".join(map(lambda x : '%02x' % x, q)))    # Create query URL
		req = urllib2.Request(target)         # Send HTTP request to server
		try:
			f = urllib2.urlopen(req)          # Wait for response
		except urllib2.HTTPError, e:          
			#print "For %d we got: %d" % (i, e.code)       # Print response code
			if e.code == 404:
				return True # good padding
			return False # bad padding

if __name__ == "__main__":
	po = PaddingOracle()
	hex_data = sys.argv[1].decode("hex")
	data = map(ord, hex_data)
	data = data[0:32]
	iv = 6
	data[15] = data[15] ^ 0x20 ^ 0x0a
	data[14] = data[14] ^ 0x73 ^ 0x0a #s
	data[13] = data[13] ^ 0x64 ^ 0x0a #d
	data[12] = data[12] ^ 0x72 ^ 0x0a #r
	data[11] = data[11] ^ 0x6f ^ 0x0a #o
	data[10] = data[10] ^ 0x57 ^ 0x0a #W
	data[9] = data[9] ^ 0x20 ^ 0x0a
	data[8] = data[8] ^ 0x63 ^ 0x0a   #c
	data[7] = data[7] ^ 0x69 ^ 0x0a   #i
	for i in range(9, 127):
		t = data[iv]
		try:
			data[iv] = data[iv] ^ i ^ 0x0a
			if po.query(i, data):
				print i
				break
		finally:
			data[iv] = t

