#include "wordle_clone/runtime_config.h"

using namespace wordle_clone;

runtime_config::runtime_config():
	strict{true}
{}

void runtime_config::add_dictionary_file(
	const std::string& _file
) {

	dictionary_files.push_back(_file);
}
