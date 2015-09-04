#!/usr/bin/env python

from  urllib.request import urlopen, Request
from urllib.parse import quote
from urllib.error import HTTPError
import sys

TARGET = 'http://crypto-class.appspot.com/po?er='
#--------------------------------------------------------------
# padding oracle
#--------------------------------------------------------------
class PaddingOracle(object):
    def query(self, i, q):
        target = TARGET + quote("".join(map(lambda x : '%02x' % x, q)))    # Create query URL
        req = Request(target)         # Send HTTP request to server
        try:
            f = urlopen(req)          # Wait for response
        except HTTPError as e:          
            #print "For %d we got: %d" % (i, e.code)       # Print response code
            if e.code == 404:
                return True # good padding
            return False # bad padding

BLOCK_SIZE = 16
result = ['*' for i in range(0, 64)]

if __name__ == "__main__":
    po = PaddingOracle()
    hex_data = bytearray( bytes.fromhex( sys.argv[1] ) )
    print( "".join(result))

    bn = 2
    pad = 1
    while bn > 1:
        data = hex_data[0 : (bn+1) * BLOCK_SIZE]
        for i in range(1, BLOCK_SIZE):
            iv = bn * BLOCK_SIZE - i
            print(iv)
            for c in range(1, 127):
                t = data[iv]
                data[iv] = data[iv] ^ c ^ pad
                if po.query(i, data):
                    print('%d %c' % (c, c))
                    result[(bn+1) * BLOCK_SIZE - iv] = c
                    for j in range(iv, bn*BLOCK_SIZE):
                        data[j] = data[j] ^ pad ^ (pad+1)
                    break
                else:
                    data[iv] = t
            pad = pad + 1
        bn = bn - 1

