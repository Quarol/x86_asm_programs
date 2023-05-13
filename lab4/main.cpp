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
        : [v1] "irm" (v1), [v2] "irm" (v2)
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
        : [v1] "irm" (v1), [v2] "irm" (v2)
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
        : [v1] "irm" (v1), [v2] "irm" (v2)
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
        : [v1] "irm" (v1), [v2] "irm" (v2)
    );
}


void add_SISD(Vector v1, Vector v2) 
{
    Vector res;

    asm (
            "fld %[a0];"
            "fadd %[b0];"
            "fstp %[x0];"

            "fld %[a1];"
            "fadd %[b1];"
            "fstp %[x1];"

            "fld %[a2];"
            "fadd %[b2];"
            "fstp %[x2];"

            "fld %[a3];"
            "fadd %[b3];"
            "fstp %[x3];"

            : [x3] "=m" (res.x3),
            [x2] "=m" (res.x2),
            [x1] "=m" (res.x1),
            [x0] "=m" (res.x0)

            : [a3] "m" (v1.x3),
            [a2] "m" (v1.x2),
            [a1] "m" (v1.x1),
            [a0] "m" (v1.x0),

            [b3] "m" (v2.x3),
            [b2] "m" (v2.x2),
            [b1] "m" (v2.x1),
            [b0] "m" (v2.x0)
    );
}


void sub_SISD(Vector v1, Vector v2)
{
    Vector res;

    asm (
            "fld %[a0];"
            "fsub %[b0];"
            "fstp %[x0];"

            "fld %[a1];"
            "fsub %[b1];"
            "fstp %[x1];"

            "fld %[a2];"
            "fsub %[b2];"
            "fstp %[x2];"

            "fld %[a3];"
            "fsub %[b3];"
            "fstp %[x3];"

            : [x3] "=m" (res.x3),
            [x2] "=m" (res.x2),
            [x1] "=m" (res.x1),
            [x0] "=m" (res.x0)

            : [a3] "m" (v1.x3),
            [a2] "m" (v1.x2),
            [a1] "m" (v1.x1),
            [a0] "m" (v1.x0),

            [b3] "m" (v2.x3),
            [b2] "m" (v2.x2),
            [b1] "m" (v2.x1),
            [b0] "m" (v2.x0)
    );
}


void mul_SISD(Vector v1, Vector v2)
{
    Vector res;

    asm (
            "fld %[a0];"
            "fmul %[b0];"
            "fstp %[x0];"

            "fld %[a1];"
            "fmul %[b1];"
            "fstp %[x1];"

            "fld %[a2];"
            "fmul %[b2];"
            "fstp %[x2];"

            "fld %[a3];"
            "fmul %[b3];"
            "fstp %[x3];"

            : [x3] "=m" (res.x3),
            [x2] "=m" (res.x2),
            [x1] "=m" (res.x1),
            [x0] "=m" (res.x0)

            : [a3] "m" (v1.x3),
            [a2] "m" (v1.x2),
            [a1] "m" (v1.x1),
            [a0] "m" (v1.x0),

            [b3] "m" (v2.x3),
            [b2] "m" (v2.x2),
            [b1] "m" (v2.x1),
            [b0] "m" (v2.x0)
    );
}


void div_SISD(Vector v1, Vector v2)
{
    Vector res;

    asm (
            "fld %[a0];"
            "fdiv %[b0];"
            "fstp %[x0];"

            "fld %[a1];"
            "fdiv %[b1];"
            "fstp %[x1];"

            "fld %[a2];"
            "fdiv %[b2];"
            "fstp %[x2];"

            "fld %[a3];"
            "fdiv %[b3];"
            "fstp %[x3];"

            : [x3] "=m" (res.x3),
            [x2] "=m" (res.x2),
            [x1] "=m" (res.x1),
            [x0] "=m" (res.x0)

            : [a3] "m" (v1.x3),
            [a2] "m" (v1.x2),
            [a1] "m" (v1.x1),
            [a0] "m" (v1.x0),

            [b3] "m" (v2.x3),
            [b2] "m" (v2.x2),
            [b1] "m" (v2.x1),
            [b0] "m" (v2.x0)
    );
}


typedef void (*function)(Vector, Vector);

double get_single_time(function f, Vector v1, Vector v2)
{
    auto start = std::chrono::high_resolution_clock::now();
    f(v1, v2);
    auto stop = std::chrono::high_resolution_clock::now();

    // result in nanoseconds
    return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
}

double get_execution_time(function f, Vector numbers1[], Vector numbers2[], int num_of_numbers)
{
    // counting loop approach
    /*
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_of_numbers; ++i)
        f(numbers1[i], numbers2[i]);
    auto stop = std::chrono::high_resolution_clock::now();

    // result in nanoseconds
    return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
    */

    
    // without counting loop approach
    double time = 0.0;
    for (int i = 0; i < num_of_numbers; ++i)
        time += get_single_time(f, numbers1[i], numbers2[i]);

    // result in nanoseconds
    return time;
}


double get_average_time(function f, Vector numbers1[], Vector numbers2[], int num_of_numbers, int repetitions)
{
    double avg = 0.0;

    for (int i = 0; i < repetitions; ++i)
        avg += get_execution_time(f, numbers1, numbers2, num_of_numbers);

    return avg / repetitions;
}


std::string generate_output(int num_of_elements1, int num_of_elements2, int num_of_elements3,
				 int repetitions, Vector v1[], Vector v2[])
{
    float addSimd, subSimd, mulSimd, divSimd;
    int elements[3] = {num_of_elements3, num_of_elements2, num_of_elements1};
    int nums_in_array;

    std::string output = "---------------------------------------------\n";
    for (int i = 0; i < 3; ++i)
    {
	nums_in_array = elements[i] / 2 / 4; 

    	addSimd = get_average_time(add_SIMD, v1, v2, nums_in_array, repetitions);
    	subSimd = get_average_time(sub_SIMD, v1, v2, nums_in_array, repetitions);
    	mulSimd = get_average_time(mul_SIMD, v1, v2, nums_in_array, repetitions);
    	divSimd = get_average_time(div_SIMD, v1, v2, nums_in_array, repetitions);

    	output += "Typ obliczen: SIMD\n";
    	output += "Liczba liczb: " + std::to_string(elements[i]) + "\n";
    	output += "Sredni czas [nanosekundy]\n";
    	output += "+ " + std::to_string(addSimd) + "\n";
    	output += "- " + std::to_string(subSimd) + "\n";
    	output += "* " + std::to_string(mulSimd) + "\n";
    	output += "/ " + std::to_string(divSimd);

	if (i != 2)
	    output += "\n\n";
    }
    output += "\n---------------------------------------------\n\n\n";


    float addSisd, subSisd, mulSisd, divSisd;


    output += "---------------------------------------------\n";
    for (int i = 0; i < 3; ++i)
    {
        nums_in_array = elements[i] / 2 / 4;

        addSisd = get_average_time(add_SISD, v1, v2, nums_in_array, repetitions);
        subSisd = get_average_time(sub_SISD, v1, v2, nums_in_array, repetitions);
        mulSisd = get_average_time(mul_SISD, v1, v2, nums_in_array, repetitions);
        divSisd = get_average_time(div_SISD, v1, v2, nums_in_array, repetitions);

    	output += "Typ obliczen: SISD\n";
    	output += "Liczba liczb: " + std::to_string(elements[i]) + "\n";
    	output += "Sredni czas [nanosekundy]\n";
    	output += "+ " + std::to_string(addSisd) + "\n";
    	output += "- " + std::to_string(subSisd) + "\n";
    	output += "* " + std::to_string(mulSisd) + "\n";
    	output += "/ " + std::to_string(divSisd);

	if (i != 2)
	    output += "\n\n";
    }
    output += "\n---------------------------------------------";


    return output;
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

    std::string output = generate_output(num_of_elements1, num_of_elements2, num_of_elements3,
				repetitions, v1, v2);

    std::ofstream file;
    file.open("results.txt");
    file << output;
    file.close();

    return 0;
}