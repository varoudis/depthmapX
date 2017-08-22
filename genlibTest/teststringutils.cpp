#include "catch.hpp"
#include "../genlib/stringutils.h"
#include "../cliTest/selfcleaningfile.h"
#include <fstream>

TEST_CASE("Tests for split function", "")
{
    {
        std::vector<std::string> stringParts = dXstring::split("foo,bar",',');
        REQUIRE(stringParts.size() == 2);
        REQUIRE(stringParts[0] == "foo");
        REQUIRE(stringParts[1] == "bar");
    }

    {
        std::vector<std::string> stringParts = dXstring::split("0.5,1.2",',');
        REQUIRE(stringParts.size() == 2);
        REQUIRE(stringParts[0] == "0.5");
        REQUIRE(stringParts[1] == "1.2");
    }

    {
        std::vector<std::string> stringParts = dXstring::split("0.5\t1.2",'\t');
        REQUIRE(stringParts.size() == 2);
        REQUIRE(stringParts[0] == "0.5");
        REQUIRE(stringParts[1] == "1.2");
    }

    {
        std::vector<std::string> stringParts = dXstring::split("0.5\t1.2\tfoo",'\t');
        REQUIRE(stringParts.size() == 3);
        REQUIRE(stringParts[0] == "0.5");
        REQUIRE(stringParts[1] == "1.2");
        REQUIRE(stringParts[2] == "foo");
    }

    {
        // skip last blank element
        std::vector<std::string> stringParts = dXstring::split("foo,bar,",',');
        REQUIRE(stringParts.size() == 2);
    }

    {
        // do not skip middle blank element
        std::vector<std::string> stringParts = dXstring::split("foo,,bar",',');
        REQUIRE(stringParts.size() == 3);
    }
}

TEST_CASE("Read String")
{
    {
        // case empty string - just read 0 length and return new string object
        SelfCleaningFile f("test.bin");
        {
            std::ofstream fs(f.Filename());
            unsigned int length = 0;
            fs.write(reinterpret_cast<char *>(&length), sizeof(unsigned int));
        }
        std::ifstream fs(f.Filename());
        auto result = dXstring::readString(fs);
        REQUIRE(result.empty());
    }

    // case non empty string - read length and then beam data into the string
    {
        SelfCleaningFile f("test.bin");
        {
            std::ofstream fs(f.Filename());
            unsigned int length = 5;
            const char *payload = "abcde";
            fs.write(reinterpret_cast<char *>(&length), sizeof(unsigned int));
            fs.write(payload, 5);
        }
        std::ifstream fs(f.Filename());
        std::string result = dXstring::readString(fs);
        REQUIRE(result == "abcde");
    }
}

TEST_CASE("Write String")
{
    {
        // case empty string - just write 0 length
        std::string testString;
        SelfCleaningFile f("test.bin");
        {
            std::ofstream fs(f.Filename());
            dXstring::writeString(fs, testString);
        }
        std::ifstream fs(f.Filename());
        unsigned int length;
        fs.read(reinterpret_cast<char *>(&length), sizeof(unsigned int));
        REQUIRE(length == 0);
        char dummy[1];
        REQUIRE_FALSE(fs.read(dummy, 1));
        REQUIRE(fs.eof());
    }

    {
        // case non empty string - just write length plus content
        std::string testString("cdfe");
        SelfCleaningFile f("test.bin");
        {
            std::ofstream fs(f.Filename());
            dXstring::writeString(fs, testString);
        }
        std::ifstream fs(f.Filename());
        unsigned int length;
        fs.read(reinterpret_cast<char *>(&length), sizeof(unsigned int));
        REQUIRE(length == testString.length());
        char buffer[5];
        buffer[4] = '\0';
        fs.read(buffer, length);
        REQUIRE(testString == buffer);
        char dummy[1];
        REQUIRE_FALSE(fs.read(dummy, 1));
        REQUIRE(fs.eof());
    }
}

#include "../genlib/paftl.h"
TEST_CASE("pstring compatibility")
{
    // case 1 write pstring and read std:string
    {
        pstring testString = "pstring";
        SelfCleaningFile f("test.bin");
        {
            std::ofstream fs(f.Filename());
            testString.write(fs);
        }
        ifstream fs(f.Filename());
        auto result = dXstring::readString(fs);
        REQUIRE(result == testString.c_str());
    }

    // case 2 write std::string and read pstring
    {
        std::string testString = "std::string";
        SelfCleaningFile f("test.bin");
        {
            std::ofstream fs(f.Filename());
            dXstring::writeString(fs, testString);
        }
        std::ifstream fs(f.Filename());
        pstring result;
        result.read(fs);
        REQUIRE(testString == result.c_str());
    }
}


TEST_CASE("test string format")
{
    REQUIRE(dXstring::formatString(1.0, "foo") == "foo");
    REQUIRE(dXstring::formatString(1.0, "%+.16le") == "+1.0000000000000000e+00");
}

TEST_CASE("test tolower")
{
    std::string tstr = "AbdUgs24*hHÜ";
    auto result = dXstring::toLower(tstr);
    REQUIRE(tstr == "abdugs24*hhÜ");
    REQUIRE(result == "abdugs24*hhÜ");
}

TEST_CASE("test ltrim")
{
    std::string normal = "   fo o ";
    dXstring::ltrim(normal);
    REQUIRE( normal == "fo o ");
    std::string empty = "";
    dXstring::ltrim(empty);
    REQUIRE( empty == "" );
    std::string justBlanks = "   ";
    dXstring::ltrim(justBlanks);
    REQUIRE( justBlanks == "");
    std::string noBlanks = "foo ";
    dXstring::ltrim(noBlanks);
    REQUIRE(noBlanks == "foo ");
}

TEST_CASE("test rtrim")
{
    std::string normal = "   fo o ";
    dXstring::ltrim(normal);
    REQUIRE( normal == "   fo o");
    std::string empty = "";
    dXstring::ltrim(empty);
    REQUIRE( empty == "" );
    std::string justBlanks = "   ";
    dXstring::ltrim(justBlanks);
    REQUIRE( justBlanks == "");
    std::string noBlanks = "foo ";
    dXstring::ltrim(noBlanks);
    REQUIRE(noBlanks == "foo");
}

TEST_CASE("test makeInitCaps")
{
    std::string tstr = "abC DEf dEf \"fOO Bar\" blah bLuB";
    dXstring::makeInitCaps(tstr);
    REQUIRE(tstr == "Abc Def Def \"fOO Bar\" Blah Blub");
}
