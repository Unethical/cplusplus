Jose Sepulveda

#include <map>
#include <iostream>

using namespace std;


//this Allocator fixes an issue of recursion, prevents the bytemap from calling new
template<class T> class MallocAllocator {
public:
    typedef T value_type;
    MallocAllocator(){}
    template<class U> MallocAllocator(const MallocAllocator<U>& other){}
    T* allocate(size_t st){
        return (T*)malloc(st * sizeof(T));
    }
    void deallocate(T* ptr, size_t st){
        free(ptr);
    }
};

class Byte_Map{
private:
    static map<void*, size_t, less<void*>, MallocAllocator<pair<void*, size_t> > > _byte_map;
public:
    static void addEntry(void* objPtr, size_t bytes){
        _byte_map.emplace(objPtr, bytes);
    }
    static size_t getEntry(void* objPtr){
        return _byte_map.at(objPtr);
    }



};
map<void*, size_t, less<void*>, MallocAllocator<pair<void*, size_t> > > Byte_Map::_byte_map;
class Byte_Counter{
private:
	static Byte_Counter* _instance;

	size_t counter;
protected:
	Byte_Counter(){
		counter = 0;
	}
public:
	~Byte_Counter(){
		free(_instance);
	}
	static Byte_Counter* Instance(){

		if (!_instance){
			//_instance = (Byte_Counter*)malloc(sizeof(Byte_Counter));
			_instance = new Byte_Counter;
			return _instance;
		}
		else{
			return _instance;
		}
	}
	void increment(size_t s){
		counter += s;
	}
	void decrement(size_t s){
		counter -= s;
	}
	void clearCounter(){
		counter = 0;
	}
	size_t getCounter(){
		return counter;
	}

	void* operator new(size_t st){
        return malloc(st);
	}

};
Byte_Counter* Byte_Counter::_instance = NULL;


void* operator new (size_t st)
{
    auto m = malloc(st);
    Byte_Map::addEntry(m, st);
	Byte_Counter::Instance()->increment(st);
	return m;
}

void* operator new [](size_t st)
{
    auto m = malloc(st);
    Byte_Map::addEntry(m, st);
	Byte_Counter::Instance()->increment(st);
	return m;
}


void operator delete(void* p) noexcept
{
	Byte_Counter::Instance()->decrement(Byte_Map::getEntry(p));
	free(p);
}

void operator delete[](void* p)
{
	Byte_Counter::Instance()->decrement(Byte_Map::getEntry(p));
	free(p);
}



void testFunction()
{
	int   *p;
	double *q;
	string *strPtr;
	int number = 10;
    strPtr = new string;
	p = new int;
	q = new double[number];

    delete p;
    delete[] q;
	delete strPtr;

}

int main()
{
    Byte_Counter::Instance()->clearCounter();

	testFunction();

	cout << Byte_Counter::Instance()->getCounter() << " byte(s) have been leaked.";

	return 0;
}
