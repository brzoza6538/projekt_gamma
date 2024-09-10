#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>


//TODO - duże lkitery?
//TODO - znaki parzyste? np. (), [], ""
//TODO - problem z linijkami z jednym znakiem na krzyż

struct pair
{
    char symbol;
    double counter;

    pair(char symbol)
    {
        this->symbol = symbol;
        counter = 1;
    }
};

std::vector<pair> dict;
int counter = 0;


void handleChar(const char& c)
{
    if(int(c) < 0 )
    {
        return;
    }
    if(c != 32 && c != 10 && (c < 'a' || c > 'z'))
    {
        return;
    }
    for(int i = 0; i < dict.size(); i++)
    {
        if(dict.at(i).symbol == c)
        {
            dict.at(i).counter++;
            counter++;
            return;
        }
    }
    dict.push_back(pair(c));
    counter++;
}

void prepare_dict()
{
    double previous = 0;
    for(int i = 0; i < dict.size(); i++)
    {
        double help = dict.at(i).counter / counter;
        dict.at(i).counter = dict.at(i).counter / counter + previous;
        previous += help;
    }
}

int binary_search(double randnum)
{
    int left = 0;
    int right = dict.size() - 1;


    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (mid == 0 || randnum <= dict.at(mid).counter && randnum > dict.at(mid - 1).counter) 
        {
            return mid;
        } 
        if (randnum > dict.at(mid).counter) 
        {
            left = mid + 1;
        } 
        else 
        {
            right = mid - 1;
        }
    }
    std::cout<<"AAAAAA"<<std::endl;
    return -1;
}

int main()
{
    std::fstream file("./tadeusz.txt");

    if(file)
    {
        std::for_each(std::istreambuf_iterator<char>(file), 
        std::istreambuf_iterator<char>(),
        handleChar);
    }

// //////////////////
//     std::cout<<counter<<"\n";
//     std::cout<<dict.size();

//     std::cout<<"\n--------------\n";

//     for(int i = 0; i < dict.size(); i++)
//     {
//         std::cout<<dict.at(i).counter<<
//         " - " << dict.at(i).symbol <<  
//         " - " << int(dict.at(i).symbol)<<"\n";
//     }
// //////////////////////

    prepare_dict();

    srand(time(0)); 

    std::ofstream plik("output.txt");
    
    for(int i = 0; i < 1000; i++)
    {
        double randnum = (double)rand() / ((double)RAND_MAX + 1);
        plik<<dict.at(binary_search(randnum)).symbol;
    }

}