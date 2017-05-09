// Copyright (C) 2017 Christian Sailer

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


#include "catch.hpp"
#include "selfcleaningfile.h"
#include <fstream>

namespace
{
    bool fileExists(const std::string &filename)
    {
        std::ifstream f(filename.c_str());
        return f.good();
    }

    bool writeToFile(const std::string &filename, const std::string &content)
    {
        std::ofstream f(filename.c_str());
        if (!f.good())
        {
            return false;
        }
        f << content;
        f.flush();
        return true;
    }
}


TEST_CASE("TestSelfCleaningFile", "Check it is deleted, doesn't fail when not present")
{
    {
        SelfCleaningFile scf("foo.txt");
        REQUIRE(scf.Filename() == "foo.txt");
        REQUIRE(fileExists("foo.txt") == false);
        REQUIRE(writeToFile(scf.Filename(), "bla bla bla"));
        REQUIRE(fileExists(scf.Filename()));
    }
    REQUIRE(fileExists("foo.txt") == false);

    {
        SelfCleaningFile scf("foo.txt");
        REQUIRE(scf.Filename() == "foo.txt");
        REQUIRE(fileExists("foo.txt") == false);
    }
    REQUIRE(fileExists("foo.txt") == false);

}

