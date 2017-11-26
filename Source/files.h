#ifndef FILES_H
#define FILES_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Files
{
public:
    static double sumFileContent(const std::string &);
    static void write(double, const std::string &);

};

#endif // FILES_H
