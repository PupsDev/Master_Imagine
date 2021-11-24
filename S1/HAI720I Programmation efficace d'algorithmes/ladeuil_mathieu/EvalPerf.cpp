#include "EvalPerf.h"
#include <sched.h>
#include <unistd.h>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

EvalPerf::EvalPerf()
{
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(0, &cpuset);
	sched_setaffinity (getpid(), sizeof(cpuset), &cpuset);

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

unsigned int EvalPerf::nb_cycle() 
{	
	return end_cycle-begin_cycle;
}
float EvalPerf::second() 
{
	return millisecond()/1000;
}
unsigned int  EvalPerf::millisecond() 
{
	return duration_cast<milliseconds>(end-begin).count();

}
unsigned int  EvalPerf::CPI(unsigned int  N) 
{
	return  nb_cycle()/N;

}