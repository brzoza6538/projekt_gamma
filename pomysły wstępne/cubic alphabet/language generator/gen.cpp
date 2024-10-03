#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

    // ró¿nica w iloœci znaków z notepad++ :
    // dochodzi do koñca, notepad liczy po prostu enter jako dwa znaki a cpp jako jeden
    //uznajemy ¿e czytamy tekst nienaukowy i nie ma zagnie¿d¿onych bramek czy czegoœ w tym guœcie
    //uznajemy ¿e nawiasy bêd¹ na pewno mia³y parê
    //œrednia d³ugoœæ nawiasów jest w incie, nie lepiej float, czy ignorowaæ niedok³adnoœci?

    //podzia³ s³owo-linia
    //œrednia d³ugoœæ linii itd
    //kropka to znak, zdania nie istniej¹
    //znaki pomiêdzy s³owami przed spacj¹ mog¹ wyst¹piæ

    //tabulatory nigdzie nie widzê w tekstach, udajemy ¿e nie ma


    //żeby nie zapamiętywać długości, lub przelatywać drugi raz, uznaję że standard diviation/sigma długości słów = 2.5

    //samogłoski - spółgłoski podział?
    //słowo -> zdanie -> linia -> akapit?
    //generuj akapit - w pętli generuj linie tak żeby długość akapitu leżała na gausie licznika podwójnego enter
    //generuj linię - w pętli generuj zdania aż prekroczysz granicę losowana ze średniej długości od enter_x1 do enter_x1
    //generuj zdanie - w pętli generuj słowa, pomiędzy sprawdzaj czy dolosowano znak (prawd = symb_c/ilość słów-1)
    // ^potrzeben? nie lepiej potraktować kropki jak koażdy inny znak?
    //generuj słowo - w pętli generuj litery według (p = let_c[let]/let_counter)

    //ilość n w n-1 według gausa (słów w zdaniu itd.)
    //szansa na dany znak zwykłym prawdopodobienstwem
    //zdania prawdopodobieństwem wystąpienia kropki plus kropka na końcu

    //może warto obliczyć ilość słów w akapicie, ilość linii w akapicie itd, a nie odzielnie jak długie są
    //nie lepiej obliczyć ile średnio mają słów linie, ile średnio linii akapity?
    //zamiast ile linii jest ogółem?
    //czy wystarczy że wiemy że średnio linia ma ileś słów!!
    //potrzebujemy wiedzieć ile ma słów!

    //ile słów w zdaniu, ile zdań w lini, ile linii w akapicie
    //znamy niby średnią długość lini w znakach
    // może lepiej żeby generować słowa aż spotkamy limit średni lini
    // limit linii uznajemy za średnią i nie przekraczamy

    // kropki stawiać w tekście losowo według prawdopodobieństwa
    // (lepiej chyba stawiać kropki na podstawie licznika słów)

    //ostatnia linia ma szansę być krótsza?



// [  ]  (  ) " "
double PI = 3.141592;

enum mirror_symbols {
    SQUARE,
    PARENTHESIS,
    QUOTE,
    M_SIZE
};

int mirror_pos_l[M_SIZE] = {91, 40, 34};
int mirror_pos_r[M_SIZE] = {93, 41, 34};

// : -
enum symbols {
    COLON,
    UNDERSCORE,
    SYM_SIZE
};

int symbols_pos[SYM_SIZE]= {58, 45};


// separators
enum separators {
    SPACE,
    ENTER,
    DOT,
    SEP_SIZE
};

const int sep_pos [SEP_SIZE] = {32, 10, 46};


//----------------------------------------

//counters
int counter = 0;
int letter_counter = 0;

double letters_c[26];
double separators_c[SEP_SIZE][3] = {0};
double symbols_c[SYM_SIZE] = {0};
double mirror_symbols_c[M_SIZE][3] = {0};
double double_enter_c[3] = {0, 0, -1};
//liczba par,œrednia d³ugoœæ, pozycja ostatniego


//----------------------------------------
void prepare_counters()
{
    for(int i = 0; i < SEP_SIZE; i++)
    {
       separators_c[i][2] = -1;
    }

}

void update_counters(int b)
{
    //enter also means word ended - update space counters
    //also count double enter separatley?
    if(b == char(sep_pos[ENTER]))
    {
        separators_c[SPACE][1] =
            (
                (separators_c[SPACE][1] * separators_c[SPACE][0]) +
                (counter - separators_c[SPACE][2] - 1)
            ) / (separators_c[SPACE][0] + 1);

        separators_c[SPACE][0] ++;
        separators_c[SPACE][2] = counter;

        if(counter - separators_c[ENTER][2] - 1 == 0)
        {
              double_enter_c[1] =
                (
                    (double_enter_c[1] * double_enter_c[0]) +
                    (counter - double_enter_c[2] - 1)
                ) / (double_enter_c[0] + 1);

            double_enter_c[0] ++;
            double_enter_c[2] = counter;
            return;
        }
    }

    //separators
    for(int symb = 0; symb < SEP_SIZE; symb++)
    {
        if(b == char(sep_pos[symb]))
        {
            separators_c[symb][1] =
                (
                    (separators_c[symb][1] * separators_c[symb][0]) +
                    (counter - separators_c[symb][2] - 1)
                ) / (separators_c[symb][0] + 1);

            separators_c[symb][0] ++;
            separators_c[symb][2] = counter;

            return;
        }
    }

    //letters
    if(int(b) >= 65 && int(b) <= 90)
    {
        b += 32;
    }
    if(int(b) >= 97 && int(b) <= 122)
    {
        letters_c[int(b) - 97] ++;
        letter_counter++;
        return;
    }

    //symbols
    for(int symb = 0; symb < SYM_SIZE; symb++)
    {
        if(b == char(symbols_pos[symb]))
        {
            symbols_c[symb] ++;

            return;
        }
    }

    //mirror symbols
    for(int symb = 0; symb < M_SIZE; symb++)
    {
        if(b == char(mirror_pos_l[symb]))
        {
            mirror_symbols_c[symb][2] = counter;
            return;
        }
        else if (b == char(mirror_pos_r[symb]))
        {
            mirror_symbols_c[symb][1] =
                (
                    (mirror_symbols_c[symb][1] * mirror_symbols_c[symb][0]) +
                    (counter - mirror_symbols_c[symb][2] - 1)
                ) / (mirror_symbols_c[symb][0] + 1);


            mirror_symbols_c[symb][0] ++;
            return;
        }
    }

}

//----------------------------------------
void prepare_letters()
{
    double previous = 0;
    for(int i = 0; i < 26; i++)
    {
        double help = letters_c[i] / letter_counter;
        letters_c[i] = letters_c[i] / letter_counter + previous;
        previous += help;
    }
}

int binary_search(double randnum)
{
    int left = 0;
    int right = 25;


    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (mid == 0 || randnum <= letters_c[mid] && randnum > letters_c[mid])
        {
            return mid;
        }
        if (randnum > letters_c[mid])
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

double normal_number_generation(mu, sigma)
{
    (double)rand()
    u1 = ((double) rand() / (RAND_MAX))
    while(u1 == 0)
    {
        u1 = ((double) rand() / (RAND_MAX))
    }
    u1 = ((double) rand() / (RAND_MAX))
}


//------------------------------------
std::string write_word(int length)
{
    for(int i = 0; i < separators_c[SPACE][1]; i++)
    {
        //double randnum = (double)rand() / ((double)RAND_MAX + 1);
        //plik<<dict.at(binary_search(randnum)).symbol;


    }
}


//-----------------------
int main()
{
    srand(time(0));

    prepare_counters();
    std::cout<<"\n--------------\n";

    std::ifstream file("./christmas carol.txt");

    char b;

    while(file.get(b))
    //for(int i = 0; i < 80; i++)
    {
        //file.get(b);
        //std::cout<<int(b) << " - " << b<<std::endl;

        update_counters(b);
        counter++;
    }
    file.close();

//-----------------------

    prepare_letters();




    std::cout<<"\n--------------\n";

    std::cout<<counter<<std::endl;
    std::cout<<letter_counter<<std::endl;
    std::cout<<letters_c[25]<<std::endl;
    std::cout<<"\n--------------\n";

    std::cout<<separators_c[0][0]  << " - " << (sep_pos[0])<<std::endl;
    std::cout<<separators_c[0][1]  << " - " << std::endl;
    std::cout<<separators_c[0][2]  << " - " << std::endl;

    std::cout<<"\n--------------\n";
    std::cout<<separators_c[1][0]  << " - " << (sep_pos[1])<<std::endl;
    std::cout<<separators_c[1][1]  << " - " << std::endl;
    std::cout<<separators_c[1][2]  << " - " << std::endl;

    std::cout<<"\n--------------\n";
    std::cout<<separators_c[2][0]  << " - " << (sep_pos[2])<<std::endl;
    std::cout<<separators_c[2][1]  << " - " << std::endl;
    std::cout<<separators_c[2][2]  << " - " << std::endl;

    std::cout<<"\n--------------\n";
    std::cout<<double_enter_c[0]  << " - " << "2x enter" <<std::endl;
    std::cout<<double_enter_c[1]  << " - " << std::endl;
    std::cout<<double_enter_c[2]  << " - " << std::endl;

/*
    std::cout<<symbols_c[0]  << " - " << char(symbols_pos[0])<<std::endl;
    std::cout<<symbols_c[1]  << " - " << char(symbols_pos[1])<< std::endl;
    std::cout<<symbols_c[2]  << " - " << char(symbols_pos[2])<< std::endl;
*/
}
