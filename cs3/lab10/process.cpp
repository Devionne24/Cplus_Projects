// Devionne Littleton
// CS44001
// 11 November 2022

#include <iostream>
#include<algorithm>
#include<cstdlib>
#include<deque>
#include<string>
#include <vector>

using std::cout; using std::endl; using std::string; using std::deque; using std::vector;

//forward class declaration
class State;

class Process {
public:
	Process();
	Process* clone() { return new Process(); }
	void dispatch();
	void block();
	void unblock();
	void suspend();
	string report();
	void changeState(State* state) { state_ = state; }

	//assigning numbers
	void numOfProcess() {
		static int num = 1;
		numProcess_ = num++;
	}
	//returns process number 
	int getNumProcess() const { return numProcess_; }
private:
	int numProcess_;
	State* state_;
};

class State {
public: 
	//virtual functions for all processes
	virtual void dispatch(Process*) {}
	virtual void block(Process*) {}
	virtual void unblock(Process*) {}
	virtual void suspend(Process*) {}
	virtual string report() = 0;
	void changeState(Process* p, State* s) {
		p->changeState(s);
	}
};

class Ready: public State {
public: 
	static State* instance() {
		static State* onlyInstance = new Ready;
		return onlyInstance;
	}
	
	void dispatch(Process*) override;
	string report() override { return "Ready"; }
private: 
	Ready() {};
	Ready(const Ready&) = delete;
	Ready& operator= (const Ready&) = delete; 
};

class Running : public State {
public: 
	//Singleton
	static State* instance() {
		static State* onlyInstance = new Running;
		return onlyInstance;
	}

	void suspend(Process*) override;
	void block(Process*) override;
	
	string report() override { return "Running"; }

private:
	Running() {};
	Running(const Running&) = delete;
	Running& operator= (const Running&) = delete;
};

class Blocked : public State {
public: 
	static State* instance() {
		static State* onlyInstance = new Blocked;
		return onlyInstance;
	}

	void unblock(Process*) override;

	string report() override { return "Blocked"; }
private:
	Blocked() {};
	Blocked(const Blocked&) = delete;
	Blocked& operator= (const Blocked&) = delete;
};

//defining functions
Process::Process() {
	state_ = Ready::instance();
	numOfProcess();
}
//changing states for each process
void Ready::dispatch(Process* p) {
	changeState(p, Running::instance());
}

void Running::suspend(Process* p) {
	changeState(p, Ready::instance());
}

void Running::block(Process* p) {
	changeState(p, Blocked::instance());
}

void Blocked::unblock(Process* p) {
	changeState(p, Ready::instance());
}

void Process::dispatch() { state_->dispatch(this); }
void Process::block() { state_->block(this); }
void Process::unblock() { state_->unblock(this); }
void Process::suspend() { state_->suspend(this); }
string Process::report() { return state_->report(); }

int main() {
	srand(time(nullptr));
	//number within ready queue 
	int pNum = 4;
	//process prototype object created  
	Process pProcess;
	Process* pProcessPtr = &pProcess;
	//creating vectors for the ready and blocked queues
	deque<Process*> readyQ;
	deque<Process*>blockedQ;

	//beginning cloning process of the process prototype
	readyQ.push_back(pProcessPtr);
	for (int i = 0; i < pNum - 1; i++) {
		readyQ.push_back(pProcess.clone());
	}

	//Beginning output for each process created and numbered
	for (int i = 0; i < readyQ.size(); i++) {
		cout << "PID " << readyQ[i]->getNumProcess() << ": created" << endl;
		cout << "PID " << readyQ[i]->getNumProcess() << " state: " << readyQ[i]->report() << endl;
	}

	cout << "---------- scheduler running -------------" << endl;
	//3 choices during runtime: suspend, block, and exit
	int runningChoices;
	//2 choices during blocktime: staying blocked or unblocking
	int blockedChoices;

	//keeps running until ready or blocked queue are empty 
	while (!blockedQ.empty() || !readyQ.empty()) {
		runningChoices = (rand() % 3) + 1;
		blockedChoices = (rand() % 2) + 1;
		if (!readyQ.empty()) {
			cout << "Dispatching..." << endl;
			readyQ.front()->dispatch();
			cout << "PID " << readyQ.front()->getNumProcess() << " state: " << readyQ.front()->report() << endl;
		}
		else if (readyQ.empty() && blockedChoices == 2) {
			cout << "The ready queue is now empty." << endl;
		}

		if (!readyQ.empty()) {
			//Suspended option
				if (runningChoices == 1) {
					cout << "Suspending..." << endl;
					readyQ.front()->suspend();
					cout << "PID " << readyQ.front()->getNumProcess() << " state: " << readyQ.front()->report() << endl;

					//Pushing process to the back then deleting it in the front
					readyQ.push_back(readyQ.front());
					readyQ.pop_front();
				}
				//Blocking option
				if (runningChoices == 2) {
					cout << "Blocking..." << endl;
					readyQ.front()->block();
					cout << "PID " << readyQ.front()->getNumProcess() << " state: " << readyQ.front()->report() << endl;
					blockedQ.push_back(readyQ.front());
					readyQ.pop_front();
				}
				//Exiting option
				if (runningChoices == 3) {
					cout << "Exiting..." << endl;
					cout << "PID " << readyQ.front()->getNumProcess() << ": exited" << endl;
					readyQ.pop_front();
				}
		}
		//begins unblocking process if blockedChoices lands on 2
		cout << endl;
		if (!blockedQ.empty() || (readyQ.empty() && !blockedQ.empty())) {
			if (blockedChoices == 2) {
				cout << "Unblocking..." << endl;
				blockedQ.front()->unblock();
				cout << "PID " << blockedQ.front()->getNumProcess() << " state: " << blockedQ.front()->report() << endl;
				readyQ.push_back(blockedQ.front());
				blockedQ.pop_front();
			}
		}
	}
	cout << "All processes exited." << endl;
	cout << "---------- scheduler done -------------";
} 
