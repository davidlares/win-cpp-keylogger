#ifndef TIMER_H
#define TIMER_H

#include <thread>
#include <chrono>

class Timer {
	// async exec
	thread Thread;
	bool Alive = false;
	// how many times we call a function
	long CallNumber = -1L; 
	// counter on how many times a certain function is called
	long repeat_count = -1L;
	// interval between function calls
	chrono::milliseconds interval = chrono::milliseconds(0);
	// contains functions
	function <void(void)> funct = nullptr; // not points to any function
	
	void SleepAndRun(){
		this_thread::sleep_for(interval);
		if(Alive){ 
			Function()(); // calling functions (returned)
		}
	}
	
	void ThreadFunc() {
		// this function will be passed to the thread
		if(CallNumber == Infinite){
			while(Alive){
				SleepAndRun();
			}
		} else {
			// controlled execution
			while(repeat_count--){
				SleepAndRun();
			}
		}
	}
	
	public:
		static const long Infinite = -1L; // manually stopped
		// constructor
		// global variables -> unable to die on certain scope
		Timer(){} 
		// other constructor
		Timer(const function<void(void)> &f) : funct(f) {}
		Timer(const function<void(void)> &f,
			  const unsigned long &i, 
			  const long repeat = Timer::Infinite) : funct(f), 
			  										 interval(chrono::milliseconds(i)), 
													 CallNumber(repeat) {}
													 
		void Start(bool Async = true){
			if(IsAlive()) {
				return; // already running
			}
			Alive = true; // start running
			repeat_count = CallNumber; // repeat_count to CallNumber variable
			if(Async) {
				Thread = thread(ThreadFunc, this);
			} else {
				this->ThreadFunc();
			}
		}		
		
		void Stop() { 
			Alive = false;
			Thread.join(); // joining thread -> block the main thread
		}
		
		// returning the Alive bool statement
		bool IsAlive() const {return Alive;}
		
		// set the number of repetance
		void RepeatCount(const long r) {
			if(Alive){
				return;
			}
			CallNumber = r;
		}
		// this is a setter that set the function to execute
		void SetFunction(const function<void(void)> &f) {
			// create the timer with the empty constructor and set the variables of it
			funct = f;
		}
		
		// getting elements
		long GetLeftCount() const {
			return repeat_count;
		}
		
		// total number of repeatance
		long RepeatCount() const {
			return CallNumber;
		}
		
		void setInterval(const unsigned long &i) {
			if(Alive){
				return;
			}
			interval = chrono::milliseconds(i);
		}
		
		// getting the count of intervals - chrono to long convertion
		unsigned long Interval() const {
			return interval.count();
		}
		
		// returning the function - expected: called and executed
		const function<void(void)> & Function() const {
			return funct;
		}
};	

#endif
