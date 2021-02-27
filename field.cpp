#include "field.h"
#include "ip.h"
#include "port.h"

using namespace std;
enum {TOT_SUBSTRINGS=4};
enum {TYPE_SUBSTR=1};
enum {PATTERN_SUBSTR=2};
enum {PACKET_SUBSTR=8};

/*
* @brief Initiate new field object using the pattern string
*/
Field::Field(String pattern) : 
	pattern(pattern) {
	this->type = this->get_type();
}

Field::~Field() {
	
}

/*
* @brief analyse the pattern string of the field object and
* @return the type of the object that the pattern needs to match to (IP/PORT)
* if the input dont match IP or PORT, return GENERIC
*/
field_type Field::get_type() const{
	String **substr = new String*[TOT_SUBSTRINGS];
	size_t tsize = 0;
	size_t *sub_size = &tsize;
	this->pattern.split("=-",substr,sub_size);
	field_type ftype = GENERIC;
	*substr[TYPE_SUBSTR] = substr[TYPE_SUBSTR]->trim();
	if(substr[TYPE_SUBSTR]->equals("port")) {
		ftype = PORT;
	} else if(substr[TYPE_SUBSTR]->equals("ip")) {
		ftype = IP;
	}
	for(size_t i=0; i<*sub_size; i++) {
		delete substr[i];
	}
	delete[] substr;
	return ftype;
}

bool Field::set_value(String val){
	val.trim();
	return 0;
}

/*
* @brief get part of a packet in val and decide if its port or ip.
* call the right match_value function acorrding to its type.
* @param val is the part of the packet we want to check.
* @return true if its match the pattern or false if its not.
*/
bool Field::match_value(String val) const{
	bool x = false;
	if(this->type == PORT) {
		Port temport(this->pattern);
		x = temport.match_value(val);
	}
	else if(this->type == IP) {
		Ip tempip(this->pattern);
		x = tempip.match_value(val);
	}
	return x;
}

/*
* @brief get a packet, check if the packet match the pattern using match_value and return true
* @param is the packet we match
* @return if its match or false if its not
*/
bool Field::match(String packet) {
	String** sub_pattern = new String*[PATTERN_SUBSTR];
	size_t t = 0;
	size_t *num_sub_pattern = &t;
	this->pattern.split("=",sub_pattern, num_sub_pattern);

	for(size_t j=0; j<*num_sub_pattern; j++){
		*sub_pattern[j] = sub_pattern[j]->trim();
	}

	String** sub_packet = new String*[PACKET_SUBSTR];
	size_t t2 = 0;
	size_t *num_sub_packet = &t2;
	packet.split(",=", sub_packet, num_sub_packet);
	
	for(size_t j=0; j<*num_sub_packet; j++){
		*sub_packet[j] = sub_packet[j]->trim();
	}
	
	for (size_t i = 0 ; i<*num_sub_packet ; i++) {
		if(sub_pattern[0]->equals(*sub_packet[i])){
			bool ret = this->match_value(*sub_packet[i+1]);
			delete sub_pattern[0];
			delete sub_pattern[1];
			delete[] sub_pattern;
			for(size_t k = 0; k<*num_sub_packet; k++){
				delete sub_packet[k];
			}
			delete[] sub_packet;
			return ret;
		}
	}
	return 0;
}