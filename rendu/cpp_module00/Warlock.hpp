#pragma once


#include <iostream>
#include <string>


class Warlock{
    private:
        std::string name;
        std::string title;
        Warlock();
        Warlock(const Warlock&);
        Warlock &operator=(const Warlock &);
    public:
        Warlock(const std::string &, const std::string &);
        ~Warlock();
        void introduce() const;
        std::string const &getName() const;
        std::string const  &getTitle() const;
        void setTitle(const std::string &t);

};