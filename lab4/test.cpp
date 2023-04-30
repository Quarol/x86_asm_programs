#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <chrono>
using namespace std::chrono;


void addition() 
{
   
}

void subtraction()
{

}

void multiplication()
{

}

void division()
{

}


typedef time_point<high_resolution_clock> TimePoint;
typedef void (*function)();


microseconds::rep get_execution_time(function f)
{
    TimePoint start = high_resolution_clock::now();

    f();
    
    TimePoint stop = high_resolution_clock::now();
    microseconds duration = duration_cast<microseconds>(stop - start);

    return duration.count();
}


float get_average_time(function f, int repetitions)
{
    float avg = 0.0;

    for (int i = 0; i < repetitions; ++i)
        avg += get_execution_time(f);

    return avg / repetitions;
}


std::string generate_output(int num_of_numbers, int repetitions)
{
    float add = get_average_time(addition, repetitions);
    float sub = get_average_time(subtraction, repetitions);
    float mul = get_average_time(multiplication, repetitions);
    float div = get_average_time(division, repetitions);

    std::string output = "";
    output += "Typ obliczen: SIMD / SISD\n";
    output += "Liczba liczb: " + std::to_string(num_of_numbers) + "\n";
    output += "Sredni czas [mikrosekundy]\n";
    output += "+ " + std::to_string(add) + "\n";
    output += "- " + std::to_string(sub) + "\n";
    output += "* " + std::to_string(mul) + "\n";
    output += "/ " + std::to_string(div);

    return output;
}


int main() 
{
    /*const int repetitions = 10;
    std::string output = "";
    
    output += generate_output(2048, repetitions);
    output += "\n\n";

    output += generate_output(4096, repetitions);
    output += "\n\n";

    output += generate_output(8192, repetitions);

    std::ofstream file;
    file.open("results.txt");
    file << output;
    file.close();*/

    

    return 0;
}
