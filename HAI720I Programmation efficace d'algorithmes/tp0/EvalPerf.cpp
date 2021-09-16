#include "EvalPerf.h"
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

EvalPerf::EvalPerf()
{

}
void EvalPerf::start()
{
	begin =  high_resolution_clock::now();
	begin_cycle = _rdtsc();
}
void EvalPerf::stop()
{
	end_cycle = _rdtsc();
	end = high_resolution_clock::now();
	
}

int EvalPerf::nb_cycle() 
{	
	return end_cycle-begin_cycle;
}
float EvalPerf::second() 
{
	return (int)(millisecond()/1000);
}
int EvalPerf::millisecond() 
{
	return duration_cast<milliseconds>(end-begin).count();

}
int EvalPerf::CPI(int N) 
{
	return  nb_cycle()/N;

}