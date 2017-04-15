// Copyright (C) 2017 Petros Koutsolampros

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "textparser.h"
#include <exception>

std::vector<Line> textParser::parseLines(std::istream& stream, char delimiter = '\t') {

    std::vector<Line> lines;

    std::string inputline;
    std::getline(stream, inputline);

    std::vector<std::string> strings = textParser::split(inputline, delimiter);

    if (strings.size() < 4)
    {
        //TODO error: badly formatted header (should contain x1, y1, x2 and y2)
        cout << "Badly formatted header (should contain x1, y1, x2 and y2)" << std::endl;
        throw std::exception();
    }

    size_t i;
    for (i = 0; i < strings.size(); i++)
    {
       if (!strings[i].empty())
       {
           std::transform(strings[i].begin(), strings[i].end(), strings[i].begin(), ::tolower);
           //strings[i].ltrim('\"');
           //strings[i].rtrim('\"');
       }
    }

    int x1col = -1, y1col = -1, x2col = -1, y2col = -1;
    for (i = 0; i < strings.size(); i++) {
        if (strings[i] == "x1")
        {
            x1col = i;
        }
        else if (strings[i] == "x2")
        {
            x2col = i;
        }
        else if (strings[i] == "y1")
        {
            y1col = i;
        }
        else if (strings[i] == "y2")
        {
            y2col = i;
        }
    }

    if(x1col == -1 || y1col == -1 || x2col == -1 || y2col == -1)
    {
        // TODO display some sort of error? exception? for now empty vector
        // badly formatted header (should contain x1, y1, x2 and y2)
        cout << "Badly formatted header (should contain x1, y1, x2 and y2)" << std::endl;
        throw std::exception();
    }

    Point2f p1, p2;

    while (!stream.eof()) {
        std::getline(stream, inputline);
        if (!inputline.empty()) {
            std::vector<std::string> strings = textParser::split(inputline, delimiter);
            if (!strings.size())
            {
                continue;
            }
            if (strings.size() < 4)
            {
                cout << "Error parsing line: " << inputline << std::endl;
                throw std::exception();
            }
            for (i = 0; i < strings.size(); i++)
            {
                if (i == x1col)
                {
                    p1.x = stringToDouble(strings[i]);
                }
                else if (i == y1col)
                {
                    p1.y = stringToDouble(strings[i]);
                }
                else if (i == x2col)
                {
                    p2.x = stringToDouble(strings[i]);
                }
                else if (i == y2col)
                {
                    p2.y = stringToDouble(strings[i]);
                }
            }
            lines.push_back(Line(p1,p2));
        }
    }
    return lines;
}

template<typename Out>

void textParser::split(const std::string &s, char delim, Out result)
{
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        *(result++) = item;
    }
}


std::vector<std::string> textParser::split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

// from: https://stackoverflow.com/questions/392981/how-can-i-convert-string-to-double-in-c
double textParser::stringToDouble( const std::string& s )
{
  std::istringstream i(s);
  double x;
  if (!(i >> x))
      throw std::exception();
  return x;
}
