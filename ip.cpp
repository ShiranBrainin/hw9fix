#include "ip.h"
using namespace std;

enum {IP_SUBSTRINGS=6};
enum {BITS_SUBSTR=5};
enum {IP_VAL_BYTES=4};

/*
* @brief constructing new Ip object from the string "pattern" using the field constructor.
*/
Ip::Ip(String pattern) : Field(pattern) {
	this->set_value(pattern);
}

/*
* @brief calculate the min and max ip address that is legal according to
* the pattern we get in val and store it in the class' private variables.
* @param val is the pattern
* @return true on sucess and false otherwise.
*/
bool Ip::set_value(String val) {
	String **substr = new String*[IP_SUBSTRINGS];
	size_t tsize  = 0;
	size_t *sub_size = &tsize;
	val.split("=./",substr,sub_size);
	
	for(size_t j=0; j<*sub_size; j++){
		*substr[j] = substr[j]->trim();
	}
	
	int num_care = substr[BITS_SUBSTR]->to_integer();
	Field field(val);
	unsigned int ip_int = 0;
	for (int i=0; i<IP_VAL_BYTES ; i++) {
		int cur_add = substr[1+i]->to_integer();
		ip_int += cur_add << (24-8*i);
	}
	unsigned int var = 0xffffffff;
	unsigned int and_var = var << (32-num_care);
	this->low = and_var & ip_int;
	if(num_care < 32){
		var = var >> num_care;
	}	else {
		var = 0;
	}
	this->high = this->low + var;
	for(int j=0;j<6;j++) {
		delete substr[j];
	}
	delete[] substr;

	return 1;
}

/*
* @brief Get the number of a ip adress (a substring of a packet) and return true if its match to the private variables of the ip object.
* @param val is the substring of packet, contains the ip address
* @return false if its not.
*/
bool Ip::match_value(String val) const{
	String **substr = new String*[IP_VAL_BYTES];
	size_t tsize = 0;
	size_t *sub_size = &tsize;
	val.split("/.",substr,sub_size);

	unsigned int ip_int = 0;
	for (int i=0; i<IP_VAL_BYTES ; i++) {
		int cur_add = substr[i]->to_integer();
		ip_int += cur_add << (24-8*i);
	}

	for(int j=0;j<IP_VAL_BYTES;j++) {
		delete substr[j];
	}
	delete[] substr;
	if (this->low <= ip_int && ip_int <= this->high) {
		return true;
	}
	return false;
}
