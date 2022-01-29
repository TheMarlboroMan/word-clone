#include "wordle_clone/char_status.h"
using namespace wordle_clone;

letter::letter(
	char _c
):
	character{c},
	status{letter::values::untyped}
{}

alphabet_status::alphabet_status() {


	status.reserve('z'-'a'+1);
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
}

letter::values alphabet_status::get(
	char _c
) const {
		//
//TODO: bad calculation.
	return status.at('z'-'a'+_c);
}
