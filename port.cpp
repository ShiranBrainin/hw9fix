#include "port.h"
using namespace std;

enum {PORT_PAT_SUBSTR=4};
enum {LOW_PORT=2};
enum {HIGH_PORT=3};

/*
* @brief constructing new Port object from the string "pattern" using the field constructor.
*/
Port::Port(String pattern):Field(pattern){
	this->set_value(pattern);
}

/*
* @brief calculate the min and max port address that is legal according to
* the pattern in val and store it in the range array.
* @param val is the pattern
* @return true on sucess and false otherwise.
*/
bool Port::set_value(String val){
	String** sub_pattern = new String*[PORT_PAT_SUBSTR];
	size_t t = 0;
	size_t *num_sub_pattern = &t;
	
	val.split("=-",sub_pattern, num_sub_pattern);
	for(size_t j=0; j<*num_sub_pattern; j++){
		*sub_pattern[j] = sub_pattern[j]->trim();
	}
		
	this->range[0] = sub_pattern[LOW_PORT]->to_integer();
	this->range[1] = sub_pattern[HIGH_PORT]->to_integer();
	for(size_t i=0; i<*num_sub_pattern; i++){
		delete sub_pattern[i];
	}
	delete[] sub_pattern;
	return true;
}

/*
*@brief Get the number of a port adrress and return true if its match to the range in the range array.
* @param val is the substring of packet, contains the port address
*@return false if its not.
*/
bool Port::match_value(String val) const{
	int port_val = val.to_integer();
	if( port_val >= this->range[0] && port_val <= this->range[1] ){
		return true;
	}
	else{
		return false;
	}
}
