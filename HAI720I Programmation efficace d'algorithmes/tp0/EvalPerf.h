#include <chrono>
#include <x86intrin.h>
class EvalPerf{


private:
	std::chrono::_V2::system_clock::time_point begin;
	std::chrono::_V2::system_clock::time_point end;
	int begin_cycle;
	int end_cycle;

public:
	EvalPerf();
	
	void start();
	void stop();

	int nb_cycle() ;
	float second() ;
	int millisecond() ;
	int CPI(int N);
};