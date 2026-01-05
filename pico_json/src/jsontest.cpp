/*
 * jsontest.c
 *
 *  Created on: 11.03.2023
 *      Author: manni4
 */

/*
 * CPP console output not available
 */

//#include <iostream>

#include <stdio.h>
#include <string.h>

#define PICOJSON_USE_INT64
#include "picojson.h"

#include <cstdlib>

extern "C"
{

void test(void)
{
    //char buffer[256];

    // cpp console IO dont work
    std::cout << "Can You see me?";

    puts("At test start\n");
    std::string json = "[ \"hello JSON\" ]";
    picojson::value v;
    std::string err = picojson::parse(v, json);
    if (!err.empty())
    {
        puts(err.c_str());
        puts("\n");
    }
    else
    {
        puts("No parsing errors\n");
    }

    puts("Start test 2\n");
    json = "{ \"A\" : 1, \"B\" : 2 }";
    err = picojson::parse(v, json);
    if (!err.empty())
    {
        puts(err.c_str());
        puts("\n");
    }
    else
    {
        puts("No parsing errors\n");
    }
    puts(v.contains("A") ? "Yes" : "No");
    double d = v.get("A").get<double>();
    printf("Get A : %f\n", d);

    //long l = v.get("A").get<int64_t>();
    //printf("Get A : %ld\n", l);

    puts(v.contains("B") ? "Yes" : "No");
    d = v.get("B").get<double>();
    printf("Get B : %f\n", d);

    puts("End test 2\n");

    // cleanup ?????
}
void provocate_uncatched_throw(void)
{
    int a = 0;
    if( a == 0)
    {
        throw ;
    }
}
void set_terminate_handler( void(*my_terminate_handler)())
{
    std::set_terminate(my_terminate_handler);
}

}
