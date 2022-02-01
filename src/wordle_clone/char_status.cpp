#include "wordle_clone/char_status.h"
using namespace wordle_clone;

letter::letter(
	char _c
):
	character{_c},
	status{letter::values::untyped}
{}

alphabet_status::alphabet_status() {


	status.reserve('z'-'a'+1);
	reset();
}

void alphabet_status::reset() {

	status.clear();

	for(char c='a'; c<='z'; c++) {

		status.push_back({c});
	}
}

void alphabet_status::mark(
	char _c,
	letter::values _val
) {

	//TODO: do not mark as misplaced if it is a hit.
	auto& item=status[_c-'a'];

	switch(_val) {

		case letter::values::misplaced:
			
			//do not allow matched letters to change status
			if(item.get_status() == letter::values::match) {

				break;
			}
		case letter::values::untyped:
		case letter::values::not_present:
		case letter::values::match:

			item.set_status(_val);
		break;
	}
}

letter::values alphabet_status::get(
	char _c
) const {

	return status.at(_c-'a').get_status();
}
