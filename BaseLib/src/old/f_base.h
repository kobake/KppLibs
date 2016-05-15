#pragma once

void iswap(int *n1,int *n2);
void *cloneMemory(const void *src,size_t size);
int adjust(int n,int min,int max);


void nputs(const char *str,int n);

int doevents();

void regulate_init();
int regulate(unsigned long _interval);


int ch_printf(int ch,const char *format,...);
void ch_printf_on(int ch);
void ch_printf_off(int ch);

int fmax(int n1,int n2);
int fmin(int n1,int n2);


