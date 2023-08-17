#ifndef RF_H
#define RF_H

#include <semaphore.h>
#include <iostream>
#include <fstream>

using namespace std;

extern string output_str;

void* writer(void* param);
void* reader(void* param);
void initing();

#endif 


