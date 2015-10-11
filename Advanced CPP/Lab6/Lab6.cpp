#include <iostream>
#include <fstream>
#include <string>
#include <cerrno>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <bitset>
#include <tr1/regex>
#include <chrono>
#include <list>
#include <pthread.h>
#include "wqueue.h"
#include "ConsumerThread.h"
using namespace std;

class File{
private:
	string content, filename;
public:
	File(string s){
		filename = s;
	}
	File(){}
	void init(){
		content = getFileContents();
	}


    string getFileContents(){
        ifstream in(filename, ios::in | ios::binary);
        if (in)
        {
            string contents;
            in.seekg(0, ios::end);
            contents.resize(in.tellg());
            in.seekg(0, ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return(contents);

        }

    cout << filename << " not found.\n";

    }
    string getContent(){
        return content;
	}
	//in order to call member of class with pthread you must define a helper static
	//funtion
	static void *threadLoader(void *context){
            cout << "loading thread\n";
            //((CLASSNAME*) object ptr)->method()
         ((File *)context)->init();
	}
};


class String_util{
public:
	static string getRawNode(string nodeName, string text){
		string s = "<" + nodeName + ">";
		size_t pos = text.find(s);
		s = "</" + nodeName + ">";
		size_t pos2 = text.find(s);
		pos2 = (pos2 - pos) + s.size();
		string str = text.substr(pos + s.size(), pos2 - s.size() * 2);
		return str;
	}
	static bool has_only_spaces(const std::string& str) {
		return str.find_first_not_of(' ') == str.npos;
	}
    static int numOfCommands(string str, string delim){
		int i = 0;
		string s = delim;
		const size_t step = s.size();
		size_t count(0);
		size_t pos(0);
		while ((pos = str.find(s, pos)) != string::npos){
			++pos;
			++i;
		}

		return i;
	}
};

class REGEX_util{
public:
	static pair<string, string> getSingleData(string text, string rgx){
		pair<string, string> p;
		regex pattern(rgx);
		sregex_token_iterator it(text.begin(), text.end(), pattern);
		sregex_token_iterator it_end;
		int j = 0;
		while (it != it_end){
			if (j == 1){
				p.first = *it;
			}
			else if (j == 2){
				p.second = *it;
			}
			++it;
			j++;
		}
		return p;
	}
};

class Patient{
    private:
        int bcode;
        vector<string> info;
    public:
        Patient(string s){
            size_t pos = s.find("<patient>");
            pair<string, string> tempP;
            string str = s.substr(0, pos-1);
            bcode = atoi(str.c_str());
            string temp = String_util::getRawNode("patient", s);
            stringstream ss(temp);
            string line;
            while(getline(ss, line)){
                tempP = (REGEX_util::getSingleData(line, "[^<^>]+"));
                if(!(tempP.first == "")){
                    info.push_back(tempP.first);
                    info.push_back(": ");
                    info.push_back(tempP.second);
                    info.push_back("\n");
                }
            }

        }
        int getBcode(){
            return bcode;
        }
        void printP(){
            for(auto v: info){
                cout << v;
            }
        }


};






 pthread_barrier_t   barrier;

class Patient_Lib{
friend class ConsumerThread;
    private:
        unordered_map<int, Patient> pMap;
        vector<int> bcodes;
        string pContent, bContent;

    public:
        Patient_Lib(File& patients, File& bcodes){
            pContent = patients.getContent();
            bContent = bcodes.getContent();

            //parseBarcodes(bcodes.getContent());
            vector<pthread_t> threads(2);
            //pthread_attr_t attr;
            //void *status;

            //pthread_attr_init(&attr);
            //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

            pthread_barrier_init (&barrier, NULL, 3);
            pthread_create(&threads[0], NULL, &Patient_Lib::threadHelperB, &(*this));
            pthread_create(&threads[1], NULL, &Patient_Lib::threadHelperP, &(*this));
            pthread_barrier_wait(&barrier);
            //pthread_attr_destroy(&attr);
            //pthread_join(threads[0], &status);
            //pthread_join(threads[1], &status);


        }
        Patient getEntry(int bCode){
            auto search = pMap.find(bCode);
            if(search != pMap.end()){
                return search->second;
            }
        }
        void addEntry(Patient p){
            pMap.emplace(p.getBcode(), p);
        }
        void printLib(){
            for(auto bcode : bcodes){
                cout << "\n" << getEntry(bcode).getBcode() << ":\n";
                getEntry(bcode).printP();
            }
        }
        int getBarcodeSize(){return bcodes.size();}
    static void *threadHelperP(void* obj){
        ((Patient_Lib *)obj)->parsePatients();
        pthread_barrier_wait (&barrier);
    }

    static void *threadHelperB(void* obj){
        ((Patient_Lib *)obj)->parseBarcodes();
        pthread_barrier_wait (&barrier);
    }
    void parseBarcodes(){
            string s = bContent;
            std::regex r("[\\S]+");
            sregex_iterator it(s.begin(), s.end(), r);
            sregex_iterator end_it;
            for(; it != end_it; ++it){
                bitset<15> b(it->str());
               bcodes.push_back(b.to_ulong());
            }
    }



   void parsePatients(){
        string s = pContent;
        string delim = "</patient>";
        int counter = 0;
        size_t pos = 0;
        int limit = String_util::numOfCommands(s, delim);
        wqueue<WorkItem*>  queue;
        ConsumerThread* thread1 = new ConsumerThread(queue);
        ConsumerThread* thread2 = new ConsumerThread(queue);
        thread1->start();
        thread2->start();
        WorkItem* item;
        while(counter != limit){
            item = new WorkItem(s.substr(pos, (s.find(delim, pos) + delim.size()+2)-pos));
            queue.add(item);
            //addEntry(p);
            pos = s.find(delim, pos) + delim.size();
            counter++;
        }
        delete item;

    }




};




int main(){
    auto start = chrono::system_clock::now();
    cout << "Loading entries please wait...\n";
    File barcode("Barcodes.txt");
    File patient("Patient.xml");

/* Threading update */
    vector<pthread_t> threads(2);
    pthread_attr_t attr;
    void *status;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&threads[0], NULL, &File::threadLoader, &patient);
    pthread_create(&threads[1], NULL, &File::threadLoader, &barcode);

    pthread_attr_destroy(&attr);
    pthread_join(threads[0], &status);
    pthread_join(threads[1], &status);

    Patient_Lib pLib(patient, barcode);




    pLib.printLib();
    auto end = chrono::system_clock::now();
    auto elapsed = chrono::duration_cast<std::chrono::seconds>(end - start);
    cout << pLib.getBarcodeSize() << " entries found in " << elapsed.count() << " seconds.\n";
    return 0;
}
