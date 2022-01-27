#pragma once
#include <string>

namespace wordle_clone {

class runtime_config {

	public:
	                    runtime_config(bool, const std::string&);
	bool                is_strict() const {return strict;}
	const std::string&  get_dictionary_file() const {return dictionary_file;}
	private: 

	bool                strict{true};
	std::string         dictionary_file;
};


}
