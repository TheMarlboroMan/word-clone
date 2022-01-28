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
	void                set_show_help(bool _val) {show_help=_val;}
	bool                is_strict() const {return strict;}
	bool                is_show_help() const {return show_help;}
	const dictionaries& get_dictionaries() const {return dictionary_files;}

	private: 

	bool                strict{true},
	                    show_help{false};
	dictionaries		dictionary_files;
};


}
