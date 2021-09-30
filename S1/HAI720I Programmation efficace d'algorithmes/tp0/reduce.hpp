#include <iostream>
#include <thread>
#include <math.h>
#include <cmath>
#include <vector>

#define OP *
#define START 1

using namespace std;

template <typename T>
void reduce(const vector<T> &V, T& res){
	res=START;
	for(size_t i=0;i<V.size();i++)
		res=res OP V[i];
}

template <typename T>
void reduce2(const vector<T> &V, T& res){
	res=START;
	size_t size = V.size();
	for(size_t i=0;i<size;i++)
		res=res OP V[i];
}

template <typename T>
void reduce3(const vector<T> &V, T& res){
	res=START;
	size_t size = V.size();
	size_t i=0;
	for(i=0;i<size-1;i+=2)
	{
		res=res OP V[i];
		res=res OP V[i+1];
	}
	for(;i<size;i++)
		res = res OP V[i];

}

template <typename T>
void reduce4(const vector<T> &V, T& res){
	res=START;
	T res2=START;
	size_t size = V.size();
	size_t i=0;
	for(i=0;i<size-1;i+=2)
	{
		res=res OP V[i];
		res2=res2 OP V[i+1];
	}
	for(;i<size;i++)
		res = res OP V[i];
	res = res + res2;
}