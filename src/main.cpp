#include "wordle_clone/runtime_config.h"
#include "wordle_clone/char_status.h"
#include <tools/arg_manager.h>
#include <tools/terminal_out.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>

void read_words(const std::string&, std::vector<std::string>&);
void show_help();
wordle_clone::runtime_config get_config(const tools::arg_manager&);

int main(int argc, char ** argv) {

	using namespace tools;

	tools::arg_manager argman{argc, argv};
	wordle_clone::runtime_config rc=get_config(argman);

	if(rc.is_show_help()) {

		show_help();
		return 0;
	}
	
	std::vector<std::string> words;
	//load words
	for(const auto& filename : rc.get_dictionaries()) {

		read_words(filename, words);
	}
	
		//TODO: be able to run with --help
	
	//abort if no words are loaded!
	if(!words.size()) {

		std::cerr<<"no words loaded, aborting"<<std::endl;
		return 1;
	}
	
	//remove duplicates
	std::sort(std::begin(words), std::end(words));
	auto it=std::unique(std::begin(words), std::end(words));
	words.erase(it);
	std::cout<<"final word count is "<<words.size()<<std::endl;

	//choose a random one
	srand(time(0));
	std::string secret_word=words[rand() % words.size()];

	//start game
	int tries=0;
	wordle_clone::alphabet_status as;

	try {
	//TODO: keep a vector of used letters
	//TODO: separate this into functional chunks
	while(true) {

		for(char c='a'; c<='z'; c++) {

			int bg=tools::bg_black, fg=tools::txt_white;

			switch(as.get(c)) {

				case wordle_clone::letter::values::untyped:
					fg=tools::txt_black;
					bg=tools::bg_white;
				break;
				case wordle_clone::letter::values::not_present:

				break;
				case wordle_clone::letter::values::misplaced:
					fg=tools::txt_white;
					bg=tools::bg_yellow;
				break;
				case wordle_clone::letter::values::match:
					fg=tools::txt_white;
					bg=tools::bg_green;
				break;
			}
			
			std::cout<<s::text_color(fg)<<s::background_color(bg)<<c<<s::reset_text();
		}
		
		//get and validate input.
		std::string input;
		std::cout<<"\n>>";
		std::getline(std::cin, input);
		if(input.size() != 5) {

			std::cout<<s::text_color(tools::txt_red)<<"just 5 letters"<<s::reset_text()<<std::endl;
			continue;
		}
	
		if(rc.is_strict() && std::end(words)==std::find(std::begin(words), std::end(words), input)) {

			std::cout<<s::text_color(tools::txt_red)<<input<<" is not a word"<<s::reset_text()<<std::endl;
			continue;
		}	

		int matches=0;
	
		//TODO: print prompt of used letters.
		std::cout<<"  ";
		
		//check user input letter by letter:
		std::string copy{secret_word};
		for(std::size_t i=0; i<5; i++) {
			
			//assume a failure
			int bg=tools::bg_black, fg=tools::txt_white;

			//check for exact position, if so remove the char from the copy
			if(input[i]==secret_word[i]) {

				copy.erase(copy.find(input[i]), 1);
				bg=tools::bg_green;
				++matches;
				as.mark(input[i], wordle_clone::letter::values::match);
			}
			else {

				//check if exists in the copy, if so remove from the copy.
				auto pos=copy.find(input[i]);
				if(std::string::npos != pos) {
					
					copy.erase(pos, 1);
					bg=tools::bg_yellow;
					fg=tools::txt_black;
					//trust this class not to mark already matched ones...
					as.mark(input[i], wordle_clone::letter::values::misplaced);
				}
				else {

					as.mark(input[i], wordle_clone::letter::values::not_present);
				}
			}
		
			//print out user input with color hints...
			std::cout<<s::text_color(fg)<<s::background_color(bg)<<input[i]<<s::reset_text();
		}

		std::cout<<std::endl;

		//decide game status
		if(matches==5) {

			std::cout<<s::text_color(txt_white)<<s::background_color(bg_green)<<"you win!!"<<s::reset_text()<<std::endl;
			break;
		}
			
		if(++tries==5) {

			std::cout<<s::text_color(txt_white)<<s::background_color(bg_red)<<"you lose, the word was "<<secret_word<<s::reset_text()<<std::endl;
			break;
		}
	}

	std::cout<<s::text_color(txt_cyan)<<"exiting"<<s::reset_text()<<std::endl;
	}
	catch(std::exception& e) {

		std::cerr<<"word: "<<secret_word<<", error:"<<e.what()<<std::endl;
		return 1;
	}
	return 0;
}

wordle_clone::runtime_config get_config(
	const tools::arg_manager& _argman
) {
	wordle_clone::runtime_config result;

	std::size_t nextarg{0};
	for(const auto& arg : _argman.get_data()) {

		++nextarg;

		if(arg=="--nostrict") {

			result.set_strict(false);
			continue;
		}

		if(arg=="--help") {

			result.set_show_help(true);
		}

		if(arg=="--dictionary") {

			if(nextarg < _argman.size()) {

				result.add_dictionary_file(_argman.get_argument(nextarg));
			}
			continue;
		}
	}
	
	//last resort, use basic words...
	if(!result.get_dictionaries().size()) {

		result.add_dictionary_file("../words"); //TODO: use env class
	}

	return result;
}

void read_words(
	const std::string& _filename,
	std::vector<std::string>& _target
) {

	std::ifstream infile{_filename, std::ifstream::in};
	if(!infile) {

		throw std::runtime_error("could not open file");
	}

	std::string word;
	while(true) {

		std::getline(infile, word);
		if(infile.eof()) {

			break;
		}

		if(word.size() != 5) {
			continue;
		}
		
		_target.push_back(word);
	}
}

void show_help() {

	std::cout<<"--help: shows this help.\n"
		"--nostrict: allows to use words outside the dictionary\n"
		"--dictionary <file>: adds a dictionary file, can be used more than once\n"
		<<std::endl;
}
