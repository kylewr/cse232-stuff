#include "Penultimate.hpp"

string Penultimate::push_back(string inp)
{
    string temp = first_;
    first_ = inp;
    return temp;
}