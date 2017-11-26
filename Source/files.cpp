#include "files.h"

#include <sstream>

double Files::sumFileContent(const std::string &filename)
{
    double sum = 0.0;
    std::ifstream in;
    in.open(filename);
    std::string line;
    while (std::getline(in, line))
    {
        double val;
        std::istringstream inss(line);
        inss >> val;
        sum += val;
    }
    return sum;
}

void Files::write(double value, const std::string &filename)
{
    std::ofstream out;
    out.open(filename, std::ofstream::app);
    out << value << std::endl;
    out.close();
}
