
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <iterator>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

using namespace std;

namespace {

unsigned char hex_to_int(const char hc) {
    unsigned char ret = 0;
    switch(hc) {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        ret = hc - '0';
        break;
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            ret = 10 + hc - 'a';
            break;
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            ret = 10 + hc - 'A';
            break;
    default:
        cerr << "Invalid hex digit " << hc << endl;
        exit(1);
    }
    return ret;
}

unsigned char to_hex(char msb, char lsb) {
    unsigned char r = 0;

    r = (hex_to_int(msb) << 4) | hex_to_int(lsb);    
    
    return r;
}
};

template<size_t SIZE>
struct hex_str {
    
    hex_str(const char * s)
        :_size(0)
    {
        int j = 0;
        int t = strlen(s);
        for( int i = 0 ;j < SIZE && i < t; i += 2) {
            _data[j++] = to_hex(s[i], s[i+1]);
        }
        _size = j;
    }

    template<size_t OTHER_SIZE>
    hex_str(const hex_str<OTHER_SIZE> & that) {
        assert(OTHER_SIZE <= SIZE);
        this._size = that._size;
        memcpy(_data, that._data, _size);
    }


    template<size_t OTHER_SIZE>
    hex_str<OTHER_SIZE> substr(int i, int j) const {
        assert(OTHER_SIZE >= (j-i));
        hex_str<OTHER_SIZE> ret;
        ret._size = j-i;
        memcpy(ret._data, _data+i, ret._size);
        return ret;
    }
    
    int getSize() const {
        return _size;
    }

    friend
    ostream & operator<<(ostream & out, const hex_str & str) {
        if(out) {
            for(int i = 0; i < str._size; i++) {
                out << hex << setw(2) <<  setfill('0') << (int)str._data[i];
            }
        }
        return out;
    }

    unsigned char & operator[](int pos) {
        assert(pos < SIZE);
        return _data[pos];
    }
    
private:
    hex_str() : _size(0) {}
    
    unsigned char _data[SIZE];
    int                      _size;
};

// (/ (- 148 21) 2)

#define URL "f20bdba6ff29eed7b046d1df9fb7000058b1ffb4210a580f748b4ac714c001bd4a61044426fb515dad3f21f18aa577c0bdf302936266926ff37dbf7035d5eeb4"
#define RESULT "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
#define BLOCK_SIZE 16

int main(int argc, char * argv[])  {
    hex_str<64> arg(URL);
    hex_str<64> result(RESULT);
    
    
    /*
    cout << URL << endl << arg << endl;
    arg[arg.getSize()-1] ^= 0xb4;
    cout << URL << endl << arg << endl;
    */

    for(int i = arg.getSize()-BLOCK_SIZE-1; i >= arg.getSize() - BLOCK_SIZE-1; i--) {
        unsigned char pad = 1;
        for(unsigned char j = 0; j != 0xFF; j++) {
            unsigned char p = arg[i];
            arg[i] ^= (j ^ pad);
            cout << "curl -s -w '%%{http_code}' http://crypto-class.appspot.com/po?er="<< arg << endl;
            arg[i] = p;
        }
        pad++;
        arg[i] ^= pad;
    }

    return 0;
}
