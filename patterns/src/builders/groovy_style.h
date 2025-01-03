//
// Created by pmrj on 03-01-2025.
//

#pragma once
#include <ostream>
using namespace std;

struct Tag {
    string name, text;
    vector<Tag> children;
    vector<pair<string, string>> attributes;

    friend std::ostream& operator<<(std::ostream& out, const Tag& tag) {
        out << "<" << tag.name;

        for (const auto& attr : tag.attributes) out << " " << attr.first << "=\"" << attr.second << "\"";

        if (tag.children.size() == 0 && tag.text.length() == 0) out << "/>" << std::endl;
        else {
            out << ">" << std::endl;
            if (tag.text.length()) out << tag.text << std::endl;

            for (const auto& child: tag.children) out << child;

            out << "</" << tag.name << ">" << std::endl;
        }

        return out;
    }
};

