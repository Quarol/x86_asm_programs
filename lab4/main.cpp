#include <iostream>
#include <fstream>
#include <random>
#include <chrono>


struct Vector
{
    float x3;
    float x2;
    float x1;
    float x0;
};


void add_SIMD(Vector v1, Vector v2) 
{
    Vector res;

    asm(
        "movups %[v1], %%xmm0;"
        "movups %[v2], %%xmm1;"
        "addps %%xmm1, %%xmm0;"
        "movups %%xmm0, %[res];"
        : [res] "=rm" (res)
        : [v1] "rm" (v1), [v2] "rm" (v2)
    );
}


void sub_SIMD(Vector v1, Vector v2)
{
    Vector res;

    asm(
        "movups %[v1], %%xmm0;"
        "movups %[v2], %%xmm1;"
        "subps %%xmm1, %%xmm0;"
        "movups %%xmm0, %[res];"
        : [res] "=rm" (res)
        : [v1] "rm" (v1), [v2] "rm" (v2)
    );
}


void mul_SIMD(Vector v1, Vector v2)
{
   Vector res;

    asm(
        "movups %[v1], %%xmm0;"
        "movups %[v2], %%xmm1;"
        "mulps %%xmm1, %%xmm0;"
        "movups %%xmm0, %[res];"
        : [res] "=rm" (res)
        : [v1] "rm" (v1), [v2] "rm" (v2)
    );
}


void div_SIMD(Vector v1, Vector v2)
{
    Vector res;

    asm(
        "movups %[v1], %%xmm0;"
        "movups %[v2], %%xmm1;"
        "divps %%xmm1, %%xmm0;"
        "movups %%xmm0, %[res];"
        : [res] "=rm" (res)
        : [v1] "rm" (v1), [v2] "rm" (v2)
    );
}


void add_SISD(float num1, float num2)
{
    float res;

    asm (
            "fld %[num1];"
            "fadd %[num2];"
            "fstp %[res];"
            : [res] "=m" (res)
            : [num1] "m" (num1), [num2] "m" (num2)
    );
}


void sub_SISD(float num1, float num2)
{
    float res;

    asm (
            "fld %[num1];"
            "fsub %[num2];"
            "fstp %[res];"
            : [res] "=m" (res)
            : [num1] "m" (num1), [num2] "m" (num2)
    );
}


void mul_SISD(float num1, float num2)
{
    float res;

    asm (
            "fld %[num1];"
            "fmul %[num2];"
            "fstp %[res];"
            : [res] "=m" (res)
            : [num1] "m" (num1), [num2] "m" (num2)
    );
}


void div_SISD(float num1, float num2)
{
    float res;

    asm (
            "fld %[num1];"
            "fdiv %[num2];"
            "fstp %[res];"
            : [res] "=m" (res)
            : [num1] "m" (num1), [num2] "m" (num2)
    );
}

template <typename T>
using function =  void(*)(T, T);

template <typename T>
double get_single_time(function<T> f, T v1, T v2)
{
    auto start = std::chrono::high_resolution_clock::now();
    f(v1, v2);
    auto stop = std::chrono::high_resolution_clock::now();

    // result in nanoseconds
    return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
}


template <typename T>
double get_execution_time(function<T> f, T numbers1[], T numbers2[], int num_of_numbers)
{
    double time = 0.0;
    for (int i = 0; i < num_of_numbers; ++i)
        time += get_single_time(f, numbers1[i], numbers2[i]);

    // result in nanoseconds
    return time;
}


template <typename T>
double get_average_time(function<T> f, T numbers1[], T numbers2[], int num_of_numbers, int repetitions)
{
    double avg = 0.0;

    for (int i = 0; i < repetitions; ++i)
        avg += get_execution_time(f, numbers1, numbers2, num_of_numbers);

    return avg / repetitions;
}


float generate_random_float(float lower_bound, float upper_bound)
{
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<float> distr(lower_bound, upper_bound);

    return distr(eng);
}


void random_fill_vector(Vector* v, float lower_bound, float upper_bound)
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


void vectorArray_to_floatArray(Vector vectors[], float floats[], int num_of_vectors)
{
    int j = 0;
    for (int i = 0; i < num_of_vectors; ++i)
    {
        floats[j++] = vectors[i].x0;
        floats[j++] = vectors[i].x1;
        floats[j++] = vectors[i].x2;
        floats[j++] = vectors[i].x3;
    }
}


std::string generate_output_SIMD(int num_of_elements1, int num_of_elements2, int num_of_elements3,
				 int repetitions, Vector v1[], Vector v2[])
{
    float add, sub, mul, div;
    int elements[3] = {num_of_elements3 / 2, num_of_elements2 / 2, num_of_elements1 / 2};
    int nums_in_array;

    std::string output = "---------------------------------------------\n";
    for (int i = 0; i < 3; ++i)
    {
	nums_in_array = elements[i] / 4;

        add = get_average_time(add_SIMD, v1, v2, nums_in_array, repetitions);
        sub = get_average_time(sub_SIMD, v1, v2, nums_in_array, repetitions);
        mul = get_average_time(mul_SIMD, v1, v2, nums_in_array, repetitions);
    	div = get_average_time(div_SIMD, v1, v2, nums_in_array, repetitions);

    	output += "Typ obliczen: SIMD\n";
    	output += "Liczba liczb: " + std::to_string(elements[i]) + "\n";
    	output += "Sredni czas [nanosekundy]\n";
    	output += "+ " + std::to_string(add) + "\n";
    	output += "- " + std::to_string(sub) + "\n";
    	output += "* " + std::to_string(mul) + "\n";
    	output += "/ " + std::to_string(div);

	if (i != 2)
	    output += "\n\n";
    }
    output += "\n---------------------------------------------\n\n\n";

    return output;
}


std::string generate_output_SISD(int num_of_elements1, int num_of_elements2, int num_of_elements3,
                                 int repetitions, float numbers1[], float numbers2[])
{
    float addSimd, subSimd, mulSimd, divSimd;
    int elements[3] = {num_of_elements3 / 2, num_of_elements2 / 2, num_of_elements1 / 2};
    int nums_in_array;
    float add, sub, mul, div;

    std::string output = "---------------------------------------------\n";
    for (int i = 0; i < 3; ++i)
    {
        nums_in_array = elements[i];

        add = get_average_time(add_SISD, numbers1, numbers2, nums_in_array, repetitions);
        sub = get_average_time(sub_SISD, numbers1, numbers2, nums_in_array, repetitions);
        mul = get_average_time(mul_SISD, numbers1, numbers2, nums_in_array, repetitions);
        div = get_average_time(div_SISD, numbers1, numbers2, nums_in_array, repetitions);

        output += "Typ obliczen: SISD\n";
        output += "Liczba liczb: " + std::to_string(elements[i]) + "\n";
        output += "Sredni czas [nanosekundy]\n";
        output += "+ " + std::to_string(add) + "\n";
        output += "- " + std::to_string(sub) + "\n";
        output += "* " + std::to_string(mul) + "\n";
        output += "/ " + std::to_string(div);

        if (i != 2)
            output += "\n\n";
    }
    output += "\n---------------------------------------------";


    return output;
}


int main() 
{
    const float upper_bound = __FLT_MAX__;
    const float lower_bound = - __FLT_MAX__;
    const int repetitions = 10;
    
    const int num_of_elements1 = 2048;
    const int num_of_elements2 = 4096;
    const int num_of_elements3 = 8192;

    int nums_in_array = num_of_elements3 / 2 / 4;

    Vector v1[nums_in_array];
    Vector v2[nums_in_array];
    random_fill_arrays(v1, v2, nums_in_array, lower_bound, upper_bound);

    float numbers1[num_of_elements3];
    float numbers2[num_of_elements3];
    vectorArray_to_floatArray(v1, numbers1, nums_in_array);
    vectorArray_to_floatArray(v2, numbers2, nums_in_array);

    std::string output = generate_output_SIMD(num_of_elements1, num_of_elements2, num_of_elements3,
				repetitions, v1, v2);
    output += generate_output_SISD(num_of_elements1, num_of_elements2, num_of_elements3,
                                   repetitions, numbers1, numbers2);

    std::ofstream file;
    file.open("results.txt");
    file << output;
    file.close();

    return 0;
}
