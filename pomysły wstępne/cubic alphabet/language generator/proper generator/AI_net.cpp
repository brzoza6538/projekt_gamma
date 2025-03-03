#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "Network.h"


int main()
{
    srand(time(0));

    std::cout<<"\n--------------\n";

    std::ifstream file("./christmas carol.txt");

    char b;

    while(file.get(b))
    //for(int i = 0; i < 80; i++)
    {
        //file.get(b);
        //std::cout<<int(b) << " - " << b<<std::endl;
    }

    std::cout << "waggggggg";

    Network net;

    file.close();
}
