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
