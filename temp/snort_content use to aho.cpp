#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "aho_corasick.hpp"

bool Content_compare(std::string packet, std::string rule[], int rulenum=1)
{
    AhoCorasick<std::string> automation;//이게 클래스?임
	// create instance
    for (int  i = 0; i < rulenum; i++)
    {
        /* code */
	automation.addPattern(rule[i]);
    }
	// get matcher object
	auto matcher = automation.matcher();

	std::vector<int> not_a_packet;
	std::copy(std::begin(packet), std::end(packet), std::back_inserter(not_a_packet));
	auto begin = std::begin(not_a_packet);
	matcher.match([&begin](const std::string& pattern, decltype(begin) iterator)
	{
        return true;
		// std::cout << "Matched \"" << pattern << "\" on position "
		// 	<< std::distance(begin, iterator) << std::endl;
		// return false;
	}, std::begin(not_a_packet), std::end(not_a_packet));

    return true;
}