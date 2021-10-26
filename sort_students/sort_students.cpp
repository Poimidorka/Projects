#include "sort_students.h"
#include <tuple>


bool greater(const Student& a, const Student& b, const SortKind& sort_kind) {
	int32_t date_compare = 2;
	int32_t name_compare = 2;
	if (std::tie(a.last_name, a.name) != std::tie(b.last_name, b.name)) {
		name_compare = (std::tie(a.last_name, a.name) > std::tie(b.last_name, b.name));
	}
	const Date& date_a = a.birth_date;
	const Date& date_b = b.birth_date;
	if (std::tie(date_a.year, date_a.month, date_a.day) !=
			std::tie(date_b.year, date_b.month, date_b.day)) {
		date_compare = (std::tie(date_a.year, date_a.month, date_a.day) > 
			std::tie(date_b.year, date_b.month, date_b.day));
	}
	if (sort_kind == SortKind::Date) {
		if (date_compare != 2) {
			return date_compare;
		}
		if (name_compare == 2) {
			name_compare = 0;
		}
		return name_compare;
	}
	else {
		if (name_compare != 2) {
			return name_compare;
		}
		if (date_compare == 2) {
			date_compare = 0;
		}
		return date_compare;		
	}
}


void SortStudents(std::vector<Student>& students, SortKind sort_kind) {
	bool was_swap = true;
	size_t epochs = 0;
	const size_t length = students.size();
 	while (was_swap) {
		was_swap = false;
		for (size_t i = 0; i + 1 + epochs < length; ++i) {
			if (greater(students[i], students[i + 1], sort_kind)) {
				std::swap(students[i], students[i + 1]);
				was_swap = true;
			}
		}
		++epochs;
	}
}
