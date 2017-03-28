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

#ifndef ARGUMENTHOLDER_H
#define ARGUMENTHOLDER_H
#include <vector>
#include <string>
class ArgumentHolder{
public:
    ArgumentHolder(std::initializer_list<std::string> l ): mArguments(l){
        for (auto& arg : mArguments) {
               mArgv.push_back(arg.data());
        }
    }

    char** argv() const{
        return (char**) mArgv.data();
    }

    size_t argc() const{
        return mArgv.size();
    }

private:
    std::vector<std::string> mArguments;
    std::vector<const char *> mArgv;
};

#endif // ARGUMENTHOLDER_H
