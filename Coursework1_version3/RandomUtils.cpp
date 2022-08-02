#include "RandomUtils.h"
#include <string>
#include <random>
#include <vector>
#include <fstream>
#include <functional>
#include <iostream>

void iterateFile(std::string fileName, std::function<void(const std::string&)> callback);

RandomUtils::RandomUtils(std::default_random_engine g)
{
    Generator = g;

    // Callback adds each line of file "Birds.txt" into bird names vector
    iterateFile("Birds.txt", [&](const std::string& str) {
        birdNames.push_back(str);
        }
    );

}

int RandomUtils::getRandomInt(int lower_bound, int upper_bound)
{
    std::uniform_int_distribution<int> distribution(lower_bound, upper_bound);

    return distribution(Generator);
}


int RandomUtils::getRandomSubgroup()
{
    return getRandomInt(0, 99);
}

char RandomUtils::getRandomGroup()
{
    return (char)getRandomInt(65, 90);;
}


std::string RandomUtils::getRandomName()
{
    if (birdNames.size() > 0)
    {
        std::string name = std::string(birdNames.at(
            getRandomInt(0, birdNames.size() - 1)
        ));

        return name;
    }

    return std::string("[default name]");
}

Date RandomUtils::getRandomTimestamp()
{
    return Date::CreateRandomDate(Date(1, 1, 2000), Date(1, 1, 2020));
}

/*
 * Adapted from: https://thispointer.com/c-how-to-read-a-file-line-by-line-into-a-vector/
 */
void iterateFile(std::string fileName, std::function<void(const std::string&)> callback)
{
    // Open the file and check
    std::ifstream in(fileName.c_str());
    if (!in)
    {
        std::cerr << "Cannot open the File : " << fileName << std::endl << "Proceeding with default names." << std::endl;
        return;
    }

    // Read file line by line.
    std::string str;
    while (std::getline(in, str))
    {
        // populate bird names vector
        callback(str);
    }
    in.close();
}