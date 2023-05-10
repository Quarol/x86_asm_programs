#include <fstream>
#include <string.h>
#include <random>
#include <math.h>
#include <chrono>


struct Vector
{
    float x3;
    float x2;
    float x1;
    float x0;
};


void addition(Vector v1, Vector v2) 
{
    Vector res;

    asm(
        "movups %[v1], %%xmm0;"
        "movups %[v2], %%xmm1;"
        "addps %%xmm1, %%xmm0;"
        "movups %%xmm0, %[res];"
        : [res] "=rm" (res)
        : [v1] "irm" (v1), [v2] "irm" (v2)
    );
}


void subtraction(Vector v1, Vector v2)
{
    Vector res;

    asm(
        "movups %[v1], %%xmm0;"
        "movups %[v2], %%xmm1;"
        "subps %%xmm1, %%xmm0;"
        "movups %%xmm0, %[res];"
        : [res] "=rm" (res)
        : [v1] "irm" (v1), [v2] "irm" (v2)
    );
}


void multiplication(Vector v1, Vector v2)
{
   Vector res;

    asm(
        "movups %[v1], %%xmm0;"
        "movups %[v2], %%xmm1;"
        "mulps %%xmm1, %%xmm0;"
        "movups %%xmm0, %[res];"
        : [res] "=rm" (res)
        : [v1] "irm" (v1), [v2] "irm" (v2)
    );
}


void division(Vector v1, Vector v2)
{
    Vector res;

    asm(
        "movups %[v1], %%xmm0;"
        "divps %[v2], %%xmm1;"
        "divps %%xmm1, %%xmm0;"
        "movups %%xmm0, %[res];"
        : [res] "=rm" (res)
        : [v1] "irm" (v1), [v2] "irm" (v2)
    );
}


typedef void (*function)(Vector, Vector);

double get_execution_time(function f, Vector numbers1[], Vector numbers2[], int num_of_numbers)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_of_numbers; ++i)
        f(numbers1[i], numbers2[i]);
    auto stop = std::chrono::high_resolution_clock::now();

    // result in nanoseconds
    return std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
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
    output += "Sredni czas [mikrosekundy]\n";
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
