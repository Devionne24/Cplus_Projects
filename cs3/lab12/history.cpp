// implements command history (multiple undos)
// demonstrates command and memento pattern
// Devionne Littleton
// 12/2/2022

#include <iostream>
#include <vector>
#include <string>
#include <stack>

using std::vector; using std::string;
using std::cout; using std::cin; using std::endl;

// receiver
class Document {
public:

	Document(const vector <string>& lines = {}) : lines_(lines) {}

	// actions
	void insert(int line, const string& str) {
		const int index = line - 1;
		if (index <= lines_.size())
			lines_.insert(lines_.begin() + index, str);
		else
			cout << "line out of range" << endl;
	}

	string remove(int line) {
		const int index = line - 1;
		string deletedLine = "";
		if (index < lines_.size()) {
			deletedLine = lines_[index];
			lines_.erase(lines_.begin() + index);
		}
		else
			cout << "line out of range" << endl;
		return deletedLine;
	}

	void show() {
		for (int i = 0; i < lines_.size(); ++i)
			cout << i + 1 << ". " << lines_[i] << endl;
	}
private:
	vector<string> lines_;
};

// abstract command
class Command {
public:
	Command(Document* doc) : doc_(doc) {}
	virtual void execute() = 0;
	virtual void unexecute() = 0;
	virtual ~Command() {}
	//pure virtual function for show, made for history
	virtual void showC() = 0;
protected:
	Document* doc_;
};

// two concrete commands
class InsertCommand : public Command {
public:
	InsertCommand(Document* doc, int line, const string& str) :
		Command(doc), line_(line), str_(str) {}
	void execute() override { doc_->insert(line_, str_); }
	void unexecute() override { doc_->remove(line_); }
	void showC() override { cout << "insert " << str_ << " at line " << line_ << "\n"; }
private:
	int line_;
	string str_;
};


class EraseCommand : public Command {
public:
	EraseCommand(Document* doc, int line) :
		Command(doc), line_(line), str_("") {}
	void execute() override { str_ = doc_->remove(line_); }
	void unexecute() override { doc_->insert(line_, str_); }
	void showC() override { cout << "erase line " << line_ << "\n"; }
private:
	int line_;
	string str_;
};

// client
class DocumentWithHistory {
public:
	DocumentWithHistory(const vector<string>& text = {}) : doc_(text) {}

	void insert(int line, string str) {
		Command* com = new InsertCommand(&doc_, line, str);
		com->execute();
		doneCommands_.push(com);
	}

	void erase(int line) {
		Command* com = new EraseCommand(&doc_, line);
		com->execute();
		doneCommands_.push(com);
	}

	void undo() {
		if (doneCommands_.size() != 0) {
			Command* com = doneCommands_.top();
			doneCommands_.pop();
			com->unexecute();
		}
		else
			cout << "no commands to undo" << endl;
	}

	void history() {
		int cnt = 1;
		std::stack<Command*> c;
		//adds to count to count up number of modifications
		while(!doneCommands_.empty()) {
			Command* com = doneCommands_.top();
			doneCommands_.pop();
			cout << cnt << ". ";
			com->showC();
			c.push(com);
			++cnt;
		}
		while (!c.empty()) {
			Command* com = c.top();
			c.pop();
			doneCommands_.push(com);
			com = nullptr;
		}
	}

	void clearHistory() {
		//clears whole stack
		while (!doneCommands_.empty()) {
			doneCommands_.pop();
		}
	}

	void redo(int line) {
		std::stack<Command*> com = doneCommands_;

		//checking so that redo can be run correctly
		if (doneCommands_.size() < line) {
			cout << "Input is not valid." << endl;
			return;
		}

		if (line != 1 && !doneCommands_.empty()) {
			for (int i = 0; i < line - 1; i++) {
				com.pop();
			}
		}
		//pushes command to top then executes
		doneCommands_.push(com.top());
		com.top()->execute();
	}

	void show() { doc_.show(); }
	class Memento* createMemento() const;
	void rollBack(class Memento*);
private:
	Document doc_;
	std::stack<Command*> doneCommands_;
};

//Memento class
class Memento {
public: 
	Memento(const Document& doc): doc_(doc) {}
	const Document& getState() const { return doc_; }
private:
	const Document doc_;
};

Memento* DocumentWithHistory::createMemento() const {
	return new Memento(doc_);
}
void DocumentWithHistory::rollBack(Memento* mem) {
	doc_ = mem->getState();
}

// invoker
int main() {
	DocumentWithHistory his({
	  "Lorem Ipsum is simply dummy text of the printing and typesetting",
	  "industry. Lorem Ipsum has been the industry's standard dummy text",
	  "ever since the 1500s, when an unknown printer took a galley of",
	  "type and scrambled it to make a type specimen book. It has",
	  "survived five centuries." });

	char option;
	do {
		his.show();
		cout << endl;

		cout << "Enter option (i)nsert line, (e)rase line, (u)ndo last command, (c)heckpoint, roll(b)ack, (h)istory, (r)edo command, or (q)uit: ";
		cin >> option;

		int line; string str;
		static Memento* checkpoint = nullptr;
		switch (option) {
		case 'i':
			cout << "line number to insert: ";
			cin >> line;
			cout << "line to insert: ";
			cin.get(); // removes end-of-line character so getline() works correctly
			getline(cin, str);
			his.insert(line, str);
			break;

		case 'e':
			cout << "line number to remove: ";
			cin >> line;
			his.erase(line);
			break;

		case 'u':
			his.undo();
			break;

		case 'c':
			//clearing history after every checkpoint
			checkpoint = his.createMemento();
			his.clearHistory();
			break;
		
		case 'b':
			if (checkpoint != nullptr) {
				his.rollBack(checkpoint);
				delete checkpoint;
				checkpoint = nullptr;
			}
			break;
		case 'h': 
			his.history();
			cout << "----------------------------" << "\n";
			break;
		case 'r':
			//shows history
			his.history();
			cout << "Which line do you want to redo?" << "\n";
			int rLine; cin >> rLine;
			his.redo(rLine);
			break;
		}

	} while (option == 'i' || option == 'e' || option == 'u' || option == 'c' || option == 'b' || option == 'h' || option == 'r' || option != 'q');
}
