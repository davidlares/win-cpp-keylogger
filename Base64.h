#ifndef BASE64_H
#define BASE64_H

#include <vector>
#include <string>

using namespace std;

namespace Base64 {
	
	string base64_encode(const string &);

	// salts -> encryption and decryption keys
	const string &SALT1 = "davidLogger.exe";
	const string &SALT2 = "_/david";
	const string &SALT3 = "m4r1c0_el_que_lo_l3a";
	
	// we will triple encoding for making the decoding a little bit harder
	string EncryptB64(string s) {
		// generating a string
		s = SALT1 + s + SALT2 + SALT3;
		// encoding
		s = base64_encode(s);
		// modifying (arbitrary)
		s.insert(10, SALT2);
		s += SALT1;
		s = base64_encode(s);
		s.insert(1, "L");
		return s;
	}
	
	const string &BASE64_CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	string base64_encode(const string &s) {
		string ret; // output
		int val = 0; 
		int bits = -6; // groups
		const unsigned int b63 = 0x3F;
		for(const auto &c : s){ 
			// auto = unkown data variable
			val = (val << 8) + c; // shifted left by 8 places 
			bits += 8; // adding 8 to the number of bits
			while(bits >= 0){
				ret.push_back(BASE64_CODES[(val >> bits) & b63]);
				bits -= 6;
			}
			if(bits > -6){
				ret.push_back(BASE64_CODES[((val << 8) >> (bits + 8)) & b63]);
			}
			while(ret.size() % 4){
				ret.push_back('=');
			} 
			return ret;
		}
	}
}

#endif
