#include "../include/runtime_config.h"

using namespace wordle_clone;

runtime_config::runtime_config(
	bool _strict,
	const std::string& _dictionary_file
):
	strict{_strict},
	dictionary_file{_dictionary_file}
{}
