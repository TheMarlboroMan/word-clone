#pragma once
#include <vector>

namespace wordle_clone {

class letter {

	public:

	enum class values {untyped, not_present, misplaced, match}r;
	                    letter(char _c);
	char                get_character() const {return character;}
	values              get_status() const {return status;}
	void                set_status(values _val) {status=_val;}

	private:

	char                character;
	values              status;
};

class alphabet_status {

	public:

	                    alphabet_status();
	void                reset();
	//${CMAKE_CURRENT_SOURCE_DIR}/char_status.cpp
	//TODO: do not mark as misplaced if we got a hit!
	void                mark(char, letter::values);
	letter::values      get(char) const;

	private:

	std::vector<letter> status;
};
}
