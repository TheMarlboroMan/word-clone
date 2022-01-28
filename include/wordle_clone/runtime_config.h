#pragma once
#include <string>
#include <vector>

namespace wordle_clone {

class runtime_config {

	public:
	using               dictionaries=std::vector<std::string>;

	                    runtime_config();
	void                add_dictionary_file(const std::string&);
	void                set_strict(bool _val) {strict=_val;}
	bool                is_strict() const {return strict;}
	const dictionaries& get_dictionaries() const {return dictionary_files;}

	private: 

	bool                strict{true};
	dictionaries		dictionary_files;
};


}
