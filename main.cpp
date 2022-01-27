#include "include/runtime_config.h"
#include "../terminal-tools/src/terminal_out.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>

/**
 *reads words from filename into target, skips any non-five letter words. Words
 *are expected to be recorded one per line with no whitespace. Throws if the
 *file cannot be opened.
 */
void read_words(
	const std::string _filename,
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

	std::cout<<"read "<<_target.size()<<" words from "<<_filename<<"..."<<std::endl;
}

int main(int argc, char ** argv) {

	using namespace tools;

	std::vector<std::string> words;

	//load words
	//TODO: be able to switch on and off strict mode.
	//TODO: be able to run with --help
	//TODO: be able to run with --dictionary 
	switch(argc) {

		case 1: read_words("words", words); break;
		case 2: read_words(argv[1], words); break;
		default:
			std::cout<<"use with no arguments for default dictionary, with a single argument for a custom one"<<std::endl;
			return 1;
	}
	
	//abort if no words are loaded!
	if(!words.size()) {

		std::cerr<<"no words loaded, aborting"<<std::endl;
		return 1;
	}

	//choose a random one
	srand(time(0));
	std::string secret_word=words[rand() % words.size()];

	//start game
	int tries=0;
	//TODO: specify at command line
	bool strict=false;

	try {
	//TODO: keep a vector of used letters
	//TODO: separate this into functional chunks
	while(true) {

		//get and validate input.
		std::string input;
		std::cout<<">>";
		std::getline(std::cin, input);
		if(input.size() != 5) {

			std::cout<<s::text_color(tools::txt_red)<<"just 5 letters"<<s::reset_text()<<std::endl;
			continue;
		}
	
		if(strict && std::end(words)==std::find(std::begin(words), std::end(words), input)) {

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
			}
			else {

				//check if exists in the copy, if so remove from the copy.
				auto pos=copy.find(input[i]);
				if(std::string::npos != pos) {
					
					copy.erase(pos, 1);
					bg=tools::bg_yellow;
					fg=tools::txt_black;
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


