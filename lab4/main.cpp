#include <fstream>
#include <string.h>
#include <chrono>
#include <random>
#include <time.h>
#include <math.h>

// 128-bit structure: 64-bit per variable
struct Vector
{
    float x3;
    float x2;
    float x1;
    float x0;
};


void addition(Vector v1, Vector v2) 
{
    Vector v3;

    asm(
        "movups %[v1], %%xmm0\n\t"
        "movups %[v2], %%xmm1\n\t"
        "addps %%xmm1, %%xmm0\n\t"
        "movups %%xmm0, %[v3]\n\t"
        : [v3] "=rm" (v3)
        : [v1] "rm" (v1), [v2] "rm" (v2)
    );
}


void subtraction(Vector v1, Vector v2)
{
    Vector v3;

    asm(
        "movups %[v1], %%xmm0\n\t"
        "movups %[v2], %%xmm1\n\t"
        "subps %%xmm1, %%xmm0\n\t"
        "movups %%xmm0, %[v3]\n\t"
        : [v3] "=rm" (v3)
        : [v1] "rm" (v1), [v2] "rm" (v2)
    );
}


void multiplication(Vector v1, Vector v2)
{
    Vector v3;

    asm(
        "movups %[v1], %%xmm0\n\t"
        "movups %[v2], %%xmm1\n\t"
        "mulps %%xmm1, %%xmm0\n\t"
        "movups %%xmm0, %[v3]\n\t"
        : [v3] "=rm" (v3)
        : [v1] "rm" (v1), [v2] "rm" (v2)
    );
}


void division(Vector v1, Vector v2)
{
    Vector v3;

    asm(
        "movups %[v1], %%xmm0\n\t"
        "movups %[v2], %%xmm1\n\t"
        "divps %%xmm1, %%xmm0\n\t"
        "movups %%xmm0, %[v3]\n\t"
        : [v3] "=rm" (v3)
        : [v1] "rm" (v1), [v2] "rm" (v2)
    );
}


typedef void (*function)(Vector, Vector);

double get_single_time(function f, Vector v1, Vector v2)
{
    clock_t start, stop;

    start = clock();
    f(v1, v2);
    stop = clock();

    // result in nanoseconds
    return ((double)stop-start) / CLOCKS_PER_SEC * pow(10, 3);
}


double get_execution_time(function f, Vector numbers1[], Vector numbers2[], int num_of_numbers)
{
    double duration = 0;
    
    for (int i = 0; i < num_of_numbers; ++i)
        duration += get_single_time(f, numbers1[i], numbers2[i]);
   
    return duration;
}


double get_average_time(function f, Vector numbers1[], Vector numbers2[], int num_of_numbers, int repetitions)
{
    double avg = 0.0;

    for (int i = 0; i < repetitions; ++i)
        avg += get_execution_time(f, numbers1, numbers2, num_of_numbers);

    return avg / repetitions;
}


double generate_random_float(double lower_bound, double upper_bound)
{
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(lower_bound, upper_bound);

    return distr(eng);
}


void random_fill_vector(Vector* v, double lower_bound, double upper_bound) 
{
    v->x3 = generate_random_float(lower_bound, upper_bound);
    v->x2 = generate_random_float(lower_bound, upper_bound);
    v->x1 = generate_random_float(lower_bound, upper_bound);
    v->x0 = generate_random_float(lower_bound, upper_bound);
}


void random_fill_arrays(Vector vectors1[], Vector vectors2[], int num_of_numbers, float lower_bound, float upper_bound)
{
    for (int i = 0; i < num_of_numbers; ++i)
    {   
        random_fill_vector(&vectors1[i], lower_bound, upper_bound);
        random_fill_vector(&vectors2[i], lower_bound, upper_bound);
    }
}


std::string generate_output(int num_of_numbers, int repetitions, float lower_bound, float upper_bound)
{
    Vector v1[num_of_numbers];
    Vector v2[num_of_numbers];
    random_fill_arrays(v1, v2, num_of_numbers, lower_bound, upper_bound);

    float add = get_average_time(addition, v1, v2, num_of_numbers, repetitions);
    float sub = get_average_time(subtraction, v1, v2, num_of_numbers, repetitions);
    float mul = get_average_time(multiplication, v1, v2, num_of_numbers, repetitions);
    float div = get_average_time(division, v1, v2, num_of_numbers, repetitions);

    std::string output = "";
    output += "Typ obliczen: SIMD\n";
    output += "Liczba liczb: " + std::to_string(num_of_numbers) + "\n";
    output += "Sredni czas [nanosekundy]\n";
    output += "+ " + std::to_string(add) + "\n";
    output += "- " + std::to_string(sub) + "\n";
    output += "* " + std::to_string(mul) + "\n";
    output += "/ " + std::to_string(div);

    return output;
}


int main() 
{
    const float upper_bound = __FLT_MAX__;
    const float lower_bound = - __FLT_MAX__;
    const int repetitions = 10;
    
    std::string output = "";
    
    output += generate_output(2048, repetitions, lower_bound, upper_bound);
    output += "\n\n";

    output += generate_output(4096, repetitions, lower_bound, upper_bound);
    output += "\n\n";

    output += generate_output(8192, repetitions, lower_bound, upper_bound);
    output += "\n\n";

    std::ofstream file;
    file.open("SIMD_res.txt");
    file << output;
    file.close();

    return 0;
}