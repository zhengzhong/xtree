//
// Created by ZHENG Zhong on 2011-06-08.
//

#include "dom_parser_example.hpp"

#include <iostream>

int main(int, char**)
{
    std::cout << "Running all xtree examples...\n";
    dom_parse_string_example();
    dom_parse_bad_xml_example();
    return 0;
}

