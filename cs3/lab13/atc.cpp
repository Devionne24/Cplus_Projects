
// mediator-based air traffic control
// Devionne Littleton
// 12/9/2022

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <cstdio>
#include <ctime>
#include <algorithm>

// for timed sleep
#include <thread>
#include <chrono>


using std::cout; using std::cin; using std::endl;
using std::string;

enum class fLogo {Delta, JetBlue, United, AirFrance, Aeroflot, Lufthansa, KLM};

// abstract mediator
class Controller {
public:
	virtual void join(class Flight*) = 0;
	virtual void leave(class Flight*) = 0;
	virtual void broadcast() = 0;
	virtual void observe() = 0;
};

//abstract flyweight
class LOGO {
public:
	virtual string image() = 0;
};

//concrete flyweights
class Delta : public LOGO {
public: 
	string image() override { return "DELTA"; }
};

class JetBlue : public LOGO {
public:
	string image() override { return "JETBLUE"; }
};

class United : public LOGO {
public: 
	string image() override { return "UNITED"; }
};

class AirFrance : public LOGO {
public:  
	string image() override { return "AIRFRANCE"; }
};

class Aeroflot : public LOGO {
public:
	string image() override { return "AEROFLOT"; }
};

class Lufthansa : public LOGO {
public: 
	string image() override { return "LUFTHANSA"; }
};

class KLM : public LOGO {
public: 
	string image() override { return "KLM"; }
};


// concrete mediator
// tower, runway clearance
class Tower : public Controller {
public:
	void join(class Flight*) override;
	void leave(class Flight*) override;
	void broadcast() override;
	void observe() override;
private:
	std::set<class Flight*> waiting_;
};


// abstract colleague
class Flight {
public:
	Flight(Controller* controller) :controller_(controller), status_(Status::waiting) {
		task_ = rand() % 2 ? Task::taxiing : Task::approaching;
		controller_->join(this);
	}
	void receive(const string& msg) {
		if (msg.find(flightNo_) != string::npos || msg.find("all") != string::npos) {
			if (msg.find("clear") != string::npos) {
				cout << flightNo_ << " roger that, ";
				cout << (task_ == Task::taxiing ? "taking off" : "landing") << endl;
				status_ = Status::cleared;
			}
			else if (msg.find("status") != string::npos) {
				cout << flightNo_
					<< (status_ == Status::waiting ? " waiting to " : " cleared to ")
					<< (task_ == Task::taxiing ? "take off" : "land") << endl;
			}
			else
				cout << "Tower, this is " << flightNo_ << " please repeat." << endl;
		}
	}
	bool isCleared() const { return status_ == Status::cleared; }
	//now printing off logo instead of regular name
	void proceed() {
		std::this_thread::sleep_for(std::chrono::seconds(rand() % 3 + 1)); // wait a while    
		cout << ". . ." << logo_->image() << numFlight_
			<< (task_ == Task::taxiing ? " takes off" : " lands")
			<< ". . ." << endl;
		controller_->leave(this);
	}

protected:
	LOGO* logo_;
	string numFlight_;
	string flightNo_;
	Controller* controller_;
	enum class Task { taxiing, approaching };  Task task_;
	enum class Status { waiting, cleared };    Status status_;
};

//factory
class LogoMaker {
public:
	static LOGO* makeLogo(fLogo f) {
		if (logos_.find(f) == logos_.end()) {
			switch (f) {
			case fLogo::Delta:
				logos_[f] = new Delta; break;
			case fLogo::JetBlue:
				logos_[f] = new JetBlue; break;
			case fLogo::United:
				logos_[f] = new United; break;
			case fLogo::AirFrance:
				logos_[f] = new AirFrance; break;
			case fLogo::Aeroflot:
				logos_[f] = new Aeroflot; break;
			case fLogo::Lufthansa:
				logos_[f] = new Lufthansa; break;
			case fLogo::KLM:
				logos_[f] = new KLM; break;
			}
		}
		return logos_[f];
	}
private:
	static std::map<fLogo, LOGO*> logos_;
};

std::map<fLogo, LOGO*> LogoMaker::logos_;

// concrete colleagues
class Airbus : public Flight {
public:
	Airbus(Tower* tower, fLogo f) : Flight(tower) {
		static const std::vector<string> companies =
		{ "AirFrance", "KLM", "Aeroflot", "Lufthansa" };
		numFlight_ = std::to_string(rand() % 1000);
		flightNo_ = companies[rand() % companies.size()] +
			numFlight_;
		if (flightNo_.find("AirFrance") != string::npos) {
			logo_ = LogoMaker::makeLogo(fLogo::AirFrance);
		}
		else if (flightNo_.find("KLM") != string::npos) {
			logo_ = LogoMaker::makeLogo(fLogo::KLM);
		}
		else if (flightNo_.find("Aeroflot") != string::npos) {
			logo_ = LogoMaker::makeLogo(fLogo::Aeroflot);
		}
		else {
			logo_ = LogoMaker::makeLogo(fLogo::Lufthansa);
		}
		cout << flightNo_ << " requesting "
			<< (task_ == Task::taxiing ? "takeoff" : "landing") << endl;
	}
};

class Boeing : public Flight {
public:
	Boeing(Tower* tower, fLogo f) : Flight(tower) {
		static const std::vector<string> companies =
		{ "Delta", "United", "JetBlue" };
		numFlight_ = std::to_string(rand() % 1000);
		flightNo_ = companies[rand() % companies.size()] + numFlight_;
		if (flightNo_.find("Delta") != string::npos) {
			logo_ = LogoMaker::makeLogo(fLogo::Delta);
		}
		else if (flightNo_.find("United") != string::npos) {
			logo_ = LogoMaker::makeLogo(fLogo::United);
		}
		else {
			logo_ = LogoMaker::makeLogo(fLogo::JetBlue);
		}
		cout << flightNo_ << " requesting "
			<< (task_ == Task::taxiing ? "takeoff" : "landing") << endl;
	}
};


// member functions for Tower
void Tower::broadcast() {
	cout << "Tower: ";
	string msg;
	getline(cin, msg);
	if (!msg.empty())
		for (auto f : waiting_) f->receive(msg);
}


void Tower::observe() {
	auto findCleared = [](Flight* f) {return f->isCleared(); };

	auto toProceed = std::find_if(waiting_.begin(), waiting_.end(), findCleared);

	while (toProceed != waiting_.end()) { // found a cleared flight
		(*toProceed)->proceed();
		toProceed = std::find_if(waiting_.begin(), waiting_.end(), findCleared);
		if (toProceed != waiting_.end())
			cout << "MAYDAY! MAYDAY! MAYDAY! " << endl; // more than a single flight cleared
	}
}


void Tower::join(Flight* f) {
	waiting_.insert(f);
}


void Tower::leave(Flight* f) {
	waiting_.erase(f);
	delete f;
}

int main() {
	srand(time(nullptr));
	Tower jfk;
	fLogo f = fLogo::Delta;
	new Boeing(&jfk, f);
	new Airbus(&jfk, f);
	new Boeing(&jfk, f);
	new Airbus(&jfk, f);

	while (true) {
		jfk.broadcast();
		jfk.observe();
		if (rand() % 2) {
			if (rand() % 2)
				new Boeing(&jfk, f);
			else
				new Airbus(&jfk, f);
		}
	}
}
