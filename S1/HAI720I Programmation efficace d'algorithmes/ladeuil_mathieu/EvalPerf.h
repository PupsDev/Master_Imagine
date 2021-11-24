#include <chrono>
#include <x86intrin.h>
class EvalPerf{


private:
	std::chrono::_V2::system_clock::time_point begin;
	std::chrono::_V2::system_clock::time_point end;
	unsigned int  begin_cycle;
	unsigned int  end_cycle;

public:
	EvalPerf();
	
	void start();
	void stop();

	unsigned int  nb_cycle() ;
	float second() ;
	unsigned int  millisecond() ;
	unsigned int  CPI(unsigned int  N);
};