#pragma once
#include <string>

Datum class, singular piece of data
It has a type
Scope is a container for Datums

starting five types:
std::int32_t
float
std::string
vec4
mat4

know these types at runtime

RTTI system will allow you to ask a question about the data type

Uses strings and integers to identify the type of an RTTI pointer instance

Use RTTI.h that has been provided to you

Datum is not templated
It will act a lot like Vector, but type-defined at runtime

enum class DatumType
{
	Unknown,
	Integer,
	Float,
	String,
	Vect4,
	Mat4,
	Pointer
};

make a union of the datum types for convenience and support from the compiler

union DatumValues
{
	std::int32_t* i;
	float* f;
	std::string* s;
	glm::vec4* v;
	glm::mat4* m;
	RTTI** p;
};

DatumValues mData;
std::uint32_t mSize;
std::uint32_t mCapacity;

DEFAULT constructor sets type enum to Unknown

Assignment operator can throw  exceptions if the type is set to a different value

SetType can only be called once
if the type is already determined, then throw an exception

SetSize will destruct data if it is outside the new size passed in

Clear should not reset the data type

SetStorage
RTTI**

Find out how to write this without switch statements

SetFromString should enforce that the Datums type is already set

Dont deference RTTI vector elements

2 / 7 / 17

THIS will increase the capacity of Datums stuff when called
Now Set should throw exceptions when passed an index out of bounds

Create a Reserve and Resize method

internal memory uses placement new to allocate

Reserve should require you to have set type first

Use @exception

Remove on Vector should use const reference rather than copy

memmove_s in remove for pulling the remaining stuff up

compound operator== in Vector to a one - liner

2 / 8 / 17

make const and non - const versions of Get

should we allow capacity to grow past size ?

void* member in the union to assign
setting external, free, and mallocing will not require switch

perhaps remove the idea of a capacity if you wish to enforce the idea of size = capacity
try new[] and delete[] if you approach it this way

SetFromString may have a switch statement

HashMap constructor should throw when bucketCount == 0

test HashFunctors

so many things wrong with HashMap...

2 / 9 / 17

Scope

HashMap of String Datum pairs
Also Vector of pointers to String Datum pairs

Insert pair into HashMap
PushBack pair pointer into Vector

Call it OrderVector

2 / 13 / 17

Scope

please

static member objects are initialized in static memory in the order they are declared.
between modules, you do not know the order in which the linker will treat them

constructor, destructor, assignment operator, and move semantics are not inherited

2 / 14 / 17

append only modifies the order array if the item appended was not currently in the HashMap

scope** item in union of Datum

guarantee that scope pointers stored in the datum are not null

hashmap classes as final

typedef ChainType::Iterator and pass a reference into Iterator constructor

find alias using const TKey& key and std::uint32_t& hashIndex(or chainIndex)

remove could take an index to prevent slist from finding after you have already found it

typedef void(Datum::*AllocateFunction)();
static const AllocateFunction AllocateFunction()

hashmaps of functions

2/15/17

Search, Orphan, Clear are the hardest methods

Add End to DatumType if you want to use arrays of function pointers

static const HashMap<DatumType, std::uint32_t> Datum::DatumTypeSizeMap = 
{
	{ Datum::DatumType::Integer, sizeof(std::int32_t) },
	{},
	...,
}

h:
typedef void(Datum::*DeleteFunction)();
static const DeleteFunction DeleteFunctions[static_cast<int>(DatumType::END)];

cpp:
const Datum::DeleteFunction Datum::DeleteFunctions[] =
{
	nullptr,
	&Datum::DeletePrimitives,
	&Datum::DeletePrimitives,
	&Datum::DeletePrimitives,
	&Datum::DeletePrimitives,
	&Datum::DeleteStrings,
	&Datum::DeletePrimitives,
	&Datum::DeletePrimitives,
}

typedef void(Datum::*CopyFunction)(const Datum& otherDatum);
static const DeleteFunction DeleteFunctions[static_cast<int>(DatumType::END)];

invoke member function pointer with:
return (this->*ToStringFunctions[static_cast<int>(mType)])(index);

this only applies if you are not using static class functions

change setfromstring int and float to use stoi and stof

2 / 15 / 17

How do we treat the parent pointer / adoption when we assign a Scope ?
Do not assign parent = otherScope.parent
Do a deep copy of the scope where you create new scopes and adopt them respectively

Does clear call clear on children ? ->NO, their destructors would be called on clear of its containing datum

How do we deal with a parent falling out of scope before a child it is responsible for deleting ?
If the child is on the stack, will it try to delete it after the parent has already cleaned it up ?

Do a deep copy when adopting ? ->NO

Assume that any scope you contain should be deleted by you

2 / 16 / 17

Refactor RTTI and Scope equality in Datum to use the RTTI Equals method

Equals check to see if pointer are equal, rhs is not nullptr and *(rhs as scope) == *this

2 / 20 / 17

Test RTTI functions on Scope

Might as well test Remove on Datum as well

make an RTTI pointer from a stack allocated Scope

is, is, typeIdInstance, TypeIdClass, As<Scope>, QueryInterface

Attributed

public inherit Attributed when making use of the class

2 / 21 / 17

2 from 5

the colors are definitely different

should not call virtual methods in constructors and destructors because:
virtual table pointer is pointing to a potentially inaccurate position in the inheritance chain

multiple inheritance by comma separation

class AY
{
	int mInt;
};

class BEE
{
	int mInt;
};

class C : public AY, public BEE
{
	void Do()
	{
		int otherInt = AY::mInt;
	}
};

if A and B derive from the same base class, the common data is replicated in C
THIS means that we need to use the scope resolution operator to refer to which instance of the data member on the base class we want

virtual inheritance means that multiple inherited base classes will share their instances of data members

for every class you inherit, you get another virtual table

class D : public B1, public B2;

D* d = new D();
B1* b1 = d;
B2* b2 = d;

b1 and b2 point to two different things!

2 / 22 / 17

rvalues and lvalue, rvalue and lvalue references on exam

casting in C++ (all four types)

reinterpret_cast only works against pointers

const_cast toggles constness

static_cast can change types as well as pointers

dynamic_cast is ? ? ? ?

2 / 23 / 17

To test, make an AttributedFoo that extends Attributed

bool IsAttribute(const std::string& key) const;

bool IsPerscribedAttribute(const std::string& key) const;

bool IsAuxillaryAttribute(const std::string& key) const;

Populate creates all string Datum pairs for this attributed

vector of signatures or get Signature

AddInternalSignature with overloads for each datum type, name default value size
AddExternalSignature with overloads for each datum type, name pointer to data size
AddNestedScope
All three of these are for prescribed attributes

operator[] for Scope to access a Scope within it to reduce number of calls

Scope& operator[](std::uint32_t) const; // His isn't const

static HashMap<std::uint64_t, std::string> where the uint is the typeid of the RTTI(Scope) and the string is the key

2 / 25 / 17

Do we copy prescribed attributes of one attribute into the auxilliary attributes of
	the assignee when doing copy semantics even if they are of differing classes?

How do we initialize the static HashMap entry for the classes list of prescribed attributes?
	THIS may be performed by the PushBack on the operator[] result of Add___Attribute methods

How do we get around the memory leak issue of having static memory that does not get freed
	until the program ends execution?

2 / 27 / 17

after move AttributedFoo should call updated external storage
use it to update the pointers of external storage to the values in the new AttributedFoo

3 / 4 / 17

Questions for Paul:

How do we ensure that a parent classes prescribed attributes are treated as prescribed attributes for the derived class?

What is the best way to make use of InitializeSignatures when we begin to derive from Attributed?
Do we call it in constructors?
We do not get polymorphic behavior from constructors so how do we ensure that the parent class prescribed attributes are appended to the derived class?
Is this a burden that the derived class will need to constantly pick up or is there a solution in Attributed itself?

->	CALL BASE CLASSES INITIALIZESIGNATURES FROM YOUR CONSTRUCTOR

3 / 6 / 17

Enhance auxiliary search by storing an index which is the first auxiliary attribute
DUPLICATE parents prescribed attribute list.

Enhance add internal attribute to accept initial values by providing overrrides

Should we be defaulting values for adding pointer attributes?

attributed should update the "this" pointer attribute during copy

3 / 7 / 17

Ensure Scope Datums are using Scope references and not Scope pointers
Helps with assignment of Datums for RTTI*s that are also scopes even if you want them as an RTTI*

Ensure that you are using references when passing strings, vectors and matrices around

initialize nullptr in datumvalue constructor rather than datum constructor

self assignment testing in Datum

test exceptions being thrown in set in Datum

test operator= for scalar assignment when it uses set in Datum

make a find contained scope method on Scope that takes a scope reference and an integer reference

3 / 8 / 17

chain of responsibility PATTERN
participants:
Abstract Handler defines interface for handling requests
Master Handler maintains list of handlers, successively delegates each request to handler, stop delegating after first handler that handles request
Concrete Handler registers itself with master handler, implements handling requests, if handles request returns true otherwise returns false
Client makes requests via master handler

(void* userData, const char* name, const char** attributes)

class SharedData : RTTI
{
	GENERATETHINGS(SharedData, RTTI);
public:
	SharedData();
	SharedData(const SharedData& otherSharedData) = delete;
	SharedData& operator=(const SharedData& otherSharedData) = delete;
	virtual ~SharedData();
	virtual void Initialize();
	virtual SharedData* Create() const = 0;
	XmlParseMaster* GetXmlParseMaster() const;
	void IncrementDepth();
	void DecrementDepth();
	std::uint32_t Depth() const;

protected:
	std::uint32_t mDepth;
	XmlParseMaster mXmlParseMaster;
}

iterators:
++, dereference, equality

not inherited:
constructors, destructor, move assignment operator, assignment operators

how is polymorphism implemented:
the virtual table which is a table of pointers to virtual methods
there is one virtual table for every class that has a virtual method in it
as well as any class that inherits from a class with a virtual table

how does the virtual table work with multiple inheritance?
the class that contains multiple base classes has multiple virtual table pointers in its data

replication vs virtual inheritance:
what are the shapes?

3 / 15 / 17

What the heck is Expat anyway???!?!?

3 / 20 / 17

SharedData is RTTI and can be PURE virtual
IXmlParseHelper should take SharedData& as arguments to the three bool methods
explicitly delete the copy and assignment operator of IXmlParseHelper
RTTI_DECLARATIONS for IXmlParseHelper

IXmlParseHelper should have an Initialize method
The first call to parse should call Initialize on shared data and all helpers in the list

How do we use RTTI in SharedData to deal with things?

3 / 21 / 17

Prevent XmlParseMaster from deleting its SharedData reference if it was manually set from SetSharedData on a cloned XmlParseMaster

XmlTableParser

class XmlParseHelperTable : IXmlParseHelper
{

public:

	class SharedData : public XmlParseMaster::SharedData
	{

	public:

		SharedData();

		virtual ~SharedData();



		Scope* GetScope() const;

	private:

		Scope* mScope;
	};

	virtual void Initialize();

	virtual bool StartElementHandler();

	virtual bool EndElementHandler();

	virtual bool CharDataHandler();

private:

	bool mParsingData;

};

Come up with a grammar:

<scope name='Level 1'>
	<entry type='integer' name='Health' value='10'/>
	<entry type='string' name='Aliases' value='Steals McGee' index='0'/>
	<entry type='string' name='Aliases' value='Sanjay' index='1'/>
	<scope name='Powers'>
		<entry type='integer' name='Power Count' value='0'/>
	</scope>
</scope>

static const std::string entryElement = "entry";
static const std::string typeAttribute = "type";
static const std::string nameAttribute = "name";
static const std::string valueAttribute = "value";
static const std::string indexAttribute = "index";

AppendScope if current scope of SharedData is not nullptr!
On EndElementHandler set parent of appended scope

new SharedData should invoke Initialize of base class when Initialize is called

element handlers should check the passed in shared data type using RTTI.Is(::TypeIdClass())
should return false if not matching type

if (mParsingData)
{
	throw std::exception("Received another start element when already parsing an element");
}

if (name != integerelementname)
{
	return false; //parser cannot handle it!
}

mParsingData = true; // this bool will disallow nested tags (something we don't want to do necessarily).

may want to create several IXmlParseHelper classes for each type of datum ?

in EndElementHandler() : shareddata.setstuff = stoi(thing);
mParsingData = false;

enum class State
{
	NotParsing,
	ParsingVector,
	ParsingX,
	ParsingY,
	ParsingZ,
	ParsingW,
};

State state;

const HashMap<State, std::string> StateNameMappings
{
	{ State::NotParsing, "none" },
	{ State::ParsingVector, "vector" },
	{ State::ParsingX, "x" },
	{ State::ParsingY, "y" },
	{ State::ParsingZ, "z" },
	{ State::ParsingW, "w" },
}

StartElementHandler()
{
	State enterState = State::NotParsing;

}

ShouldHandleTag(const string& name, State startingState)
{
	if (in SateNameMappings)
	{
		state = NameStateMappings[name];
		return true;
	}

	return false;
}

<scope name = 'Level 1'>
<integer name = 'Health' value = '10' / >
<string name = 'Aliases' value = 'Steals McGee' index = '0' / >
<string name = 'Aliases' value = 'Sanjay' index = '1' / >
<scope name = 'Powers'>
<entry type = 'integer' name = 'Power Count' value = '0' / >
< / scope>
< / scope>

3 / 22 / 17

ExtractScope method for ScopeSharedData that returns a COPY of the scope!
Now the ScopeSharedData is allowed to destroy its own thing but you still keep the Scope information at runtime!

test for depth = 0 at end of parsing!

is FactoryManager a singleton ?

AbstractFactory(interface for creating abstract products)

ConcreteFactory(implements AbstractFactory, creates concrete products)

AbstractProduct(interface for products)

ConcreteProduct(implements AbstractProduct)

Client(uses only abstract products)

RTTI* r = Factory<RTTI>::Create("widget");

class Widget : public RTTI
{
	// whatever you want
};

// Make a macro CREATE_FACTORY(type1, type2) that creates this code for the given types
class WidgetFactory : public Factory<RTTI>
{

	WidgetFactory()
	{
		Add(this); // call to static method
	}

	~WidgetFactory()
	{
		Remove(this); // call to static method
	}

	const std::string name()
	{
		return "widget";
	}

	virtual RTTI* Create()
	{
		return new Widget();
	}
};

use macro magic! use a macro that has an argument of "widget" and new Widget()

create Factory.h
create Factory.cpp

put macros for the factories in Factory.h:

CREATE_FACTORY(RTTI, Foo);
CREATE_FACTORY(RTTI, Scope);

RTTI* r = Factory<RTTI>::Create("Foo");
RTTI* r = Factory<RTTI>::Create("Scope");

3 / 23 / 17

Have shared data and master pointers in ParseHelpers and XmlParseMaster and SharedData
make use of references in method calls rather than pointers

XmlParseMaster can take in a SharedData reference for convenience

3 / 27 / 17

Perfect Forwarding

3 / 28 / 17

Entity

World and Sector are final

Entity grammar:

<world name="Paulzworld">
	<sector name="Awesometown">
		<entity class="MonsterEntity" name="Paul">
			<integer />
			<string />
			<vector />
		</entity>
	</sector>
</world>

Have a GROUP of factories for Entities and Actions

parsing should only change for world and sector?

create WorldState class:
current World, Sector, Entity, and Action as public pointer data members
with methods for GetGameTime, SetGameTime where GameTime objects maintain total time since the game started
and elapsed time since the last iteration through the Run loop

Attributed Notes:

calling virtual function in constructor

refactor the InitializeSignatures method as a protected that calls your parents version

self assignment testing for attributed assignment, copy construction, move assignment, move construction

Equals should test for null

self assignment testing for move semantics on all the new stuff you added I guess

you are a dumb idiot

you do not deserve to exist

off you are self

3 / 29 / 17

Replace method on Scope given an old key and a new key

keep a count of elements in a scope for ensuring no collisions on the scope?

Action notes:

entity
	actionList
		action

Strategy design PATTERN:
Define a family of algorithms, encapsulate each one and make them interchangeable.
Lets algorithms vary independently from clients that use it.

Command PATTERN

Composite PATTERN:
Compose objects into tree structures to represent part-whole hierarchies.
Composite lets clients treat individual objects and compositions of objects uniformly.

3 / 30 / 17

test typeidinstance typeidclass all those methods

use equals from Attributed?

create method on Attributed that adds an empty datum of type table rather than using AddNestedScope

shunting yard algorithm

4 / 4 / 17

Maybe make a method to set the Then and Else actions of your ActionListIf

Presentation of final project:
Mini post-mortem
Game demo
Description of what you did / challenges you faced

Have an IActionContainer class that inherits from Attributed and has the methods CreateAction and Actions()
or it wont work since multiple inheritance kills RTTI for ActionList

Have your Sector and World classes invoke the Update methods of their Actions()

Try to have ActionDestroyAction recursively search up the heirarchy for the Action to delete

Create arithmetic operations like this where target is found at runtime on update with a Search() call:
<action class='IncrementAction' name='Incrementor' target='X'>
	<integer name='X' value='10'/>
</action>
If you memmoize the call to Search(), be aware that the thing may no longer exist on the next frame:
It may have been deleted between Update() calls.

Observer PATTERN

XmlParseMaster notes:

const HashMap reference in signatures
use SharedData references in signatures
use IXmlParseHelper references in signatures

in SharedData Initialize, mDepth = 0
in FooSharedData Initialize, call SharedData::Initialize()

move IncrementDepth to before iterating over helpers

magicleap
igo
ea

mock interview with paul and maybe tom in may that he does with everyone

4 / 5 / 17

Event = Observer design PATTERN

Publisher = subject
Subscriber = observer

Abstract subject, Abstract observer, Concrete subject, Concrete observer

Mediator design PATTERN
encapsulates how objects interact so they do not need to know as much about each other

Abstract mediator, Concrete mediator, Colleague classes

killme

Event class, EventQueue class, EventPublisher class, EventSubscriber class

store a pointer to a list of subscribers in event class passed to constructor

4 / 6 / 17

EventSubscriber abstract base class that will be inherited from when an object wants to get an event

Expression:
hashmap of special character to function pointer
exaluate method that iterates through the characters in the string
	adding values to a buffer string if they are not keys in the function hashmap

4 / 10 / 17

RAII research acquisition is initialization

std::unique_ptr instead of raw pointers

Reaction due friday? I dont remember him talking about it at all...
Maybe I missed it since I was 20 minutes late

Do Expression lol

4 / 11 / 17

async

#include <thread>

const int iterations = 1000;

void worker()
{
	for (int i = 0; i < iterations; ++i)
	{
		cout << "-";
	}
}

it is possible to capture all data by value in scope by using = in lambda capture list:

[=] { i + 2; }
rather than
[i]{ i + 2; }
by reference:
[&] { i + 2; }

hashmap of string to std::pair<datum type, state> for primitive parsing functions

tuple is an n-sized pair

Factory
const reference returned from GetClassName

pass factory pointer by reference in Add and Remove

4 /12 / 17

create a factory macro for reactions

pass WorldState rather than World

do not have World contain a WorldState

// ThreadTest.cpp : Defines the entry point for the console application.

#include "pch.h"
#include <thread>
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <functional>
#include <sstream>
#include <future>
#include <asyncinfo.h>
#include <experimental/filesystem>
#include <algorithm>

using namespace std;
using namespace std::chrono;
using namespace experimental::filesystem::v1;
using namespace std::tr2::sys;

//const int iterations = 1000;
//
//void worker()
//{
//	for (int i = 0; i < iterations; ++i)
//	{
//		std::cout << "-";
//	}
//}
//
//int main()
//{
//	std::thread t(worker);
//
//	for (int i = 0; i < iterations; ++i)
//	{
//		std::cout << "+";
//	}
//
//	t.join();
//
//	std::cout << "Finished." << std::endl;
//
//	return 0;
//}

//int main(int argc, char* argv[])
//{
//	static std::random_device device;
//	static std::default_random_engine generator(device());
//	static std::uniform_int_distribution<long> distribution(1, 1000);
//
//	std::vector<std::thread> threads;
//	const int threadCount = 16;
//
//	for (int i = 0; i < threadCount; ++i)
//	{
//		threads.emplace_back([i]
//		{
//			std::chrono::milliseconds sleepAmount(distribution(generator));
//			std::this_thread::sleep_for(sleepAmount);
//
//			std::stringstream message;
//			message << "Hello from thread" << i << std::endl;
//			std::cout << message.str();
//		});
//	}
//
//	std::stringstream message;
//	message << "Hello from main!" << std::endl;
//	std::cout << message.str();
//
//	for (auto& t : threads)
//	{
//		t.join();
//	}
//
//	std::cout << "Finished." << std::endl;
//
//	system("PAUSE");
//
//	return 0;
//}
//
//void repeat(std::string& str, int n)
//{
//	if (n > 0)
//	{
//		std::stringstream message;
//		message << str << std::endl;
//		std::cout << message.str();
//
//		repeat(str, n - 1);
//	}
//}
//
//int main(int argc, char* argv[])
//{
//	std::string str("Hello from thread!");
//	std::thread t(repeat, str, 3);
//	//std::thread t(repeat, ref(str), 3);
//
//	std::cout << "Hello from main!" << std::endl;
//
//	t.join();
//
//	std::cout << "Finished." << std::endl;
//
//	return 0;
//}


//void worker(std::promise<std::string>& pr)
//{
//	std::cout << "Worker sending.\n";
//	pr.set_value("Message from thread.");
//}
//
//int main(int argc, char* argv[])
//{
//	std::promise<std::string> pr;	// promise should only be used once per future get call
//	std::future<std::string> fut = pr.get_future(); // future should only be called once
//
//	std::thread t(worker, std::ref(pr));
//
//	std::cout << "Main receiving.\n";
//
//	std::string str = fut.get(); // blocks until set_value is called
//	std::cout << str << std::endl;
//
//	t.join();	// blocks until worker finished execution
//
//	std::cout << "Finished." << std::endl;
//
//	return 0;
//}

//void worker(std::promise<std::string>& pr)
//{
//	try
//	{
//		std::cout << "Worker throwing.\n";
//
//		throw std::exception("Exception from thread.");
//
//		pr.set_value("Message from thread.");
//	}
//	catch (...)
//	{
//		pr.set_exception(std::current_exception());
//	}
//}
//
//int main(int argc, char* argv[])
//{
//	std::promise<std::string> pr;	// promise should only be used once per future get call
//	std::future<std::string> fut = pr.get_future(); // future should only be called once
//
//	std::thread t(worker, std::move(pr));
//
//	std::cout << "Main receiving.\n";
//
//	try
//	{
//		std::string str = fut.get(); // blocks until set_value is called
//		std::cout << str << std::endl;
//	}
//	catch (std::exception ex)
//	{
//		std::cout << "Caught: " << ex.what() << std::endl;
//	}
//
//	t.join();	// blocks until worker finished execution
//
//	std::cout << "Finished." << std::endl;
//
//	return 0;
//}

//std::string worker(bool value)
//{
//	if (value)
//	{
//		std::cout << "Worker returning.\n";
//		return "Message from thread.";
//	}
//	else
//	{
//		std::cout << "Worker throwing.\n";
//		throw std::exception("Exception from thread.");
//	}
//}
//
//int main(int argc, char* argv[])
//{
//	std::future<std::string> fut = std::async(worker, false);
//
//	try
//	{
//		std::string str = fut.get(); // blocks until set_value is called
//		std::cout << str << std::endl;
//	}
//	catch (std::exception ex)
//	{
//		std::cout << "Caught: " << ex.what() << std::endl;
//	}
//
//	std::cout << "Finished." << std::endl;
//
//	return 0;
//}

//class Counter
//{
//public:
//	Counter(int value) : mValue(value) {}
//
//	int mValue;
//};
//
//int main(int argc, char* argv[])
//{
//	std::shared_ptr<Counter> counter = std::make_shared<Counter>(10);
//	std::future<void> fut = std::async([](std::shared_ptr<Counter> a)
//	{
//		++(a->mValue);
//	}, counter);
//
//	std::this_thread::sleep_for(std::chrono::milliseconds(10));
//
//	std::cout << "Counter: " << counter->mValue << std::endl;
//
//	fut.wait();
//
//	std::cout << "Counter: " << counter->mValue << std::endl;
//
//	std::cout << "Finished." << std::endl;
//
//	return 0;
//}

//vector<string> listDirectory(const path& directory)
//{
//	vector<string> files;
//
//	for (directory_iterator it(directory); it != directory_iterator(); ++it)
//	{
//		if (is_regular_file(it->status()))
//		{
//			files.push_back(it->path().filename().string());
//		}
//	}
//}
//
//vector<string> listDirectories(const vector<path>& paths)
//{
//	vector<future<vector<string>>> futures;
//
//	for (const auto& path : paths)
//	{
//		cout << path << endl;
//		futures.emplace_back(async(listDirectory, ref(path)));
//	}
//
//	vector<string> allFiles;
//	for (auto& f : futures)
//	{
//		vector<string> files = f.get();
//		move(files.begin(), files.end(), back_inserter(allFiles));
//	}
//	
//	return allFiles;
//}
//
//int main(int argc, char* argv[])
//{
//	std::vector<path> paths;
//
//	for (directory_iterator it("C://"); it != directory_iterator(); ++it)
//	{
//		if (is_directory(it->status()))
//		{
//			paths.push_back(it->path());
//		}
//	}
//
//	for (const auto& file : listDirectories(paths))
//	{
//		cout << file << endl;
//	}
//
//	cout << "Finished." << endl;
//
//	return 0;
//}


4 / 12 / 17


4 / 13 / 17

kill me

//class FileMonitor
//{
//public:
//	void push_back(const string& path)
//	{
//		lock_guard<mutex> lock(mMutex);
//		mFiles.push_back(path);
//	}
//
//	void print_files()
//	{
//		lock_guard<mutex> lock(mMutex);
//
//		for (const auto& file : mFiles)
//		{
//			cout << file << endl;
//		}
//	}
//
//private:
//	mutex mMutex;
//	vector<string> mFiles;
//};
//
//void listDirectory(const path& directory, FileMonitor& fileSink)
//{
//	for (directory_iterator it(directory); it != directory_iterator(); ++it)
//	{
//		if (is_regular_file(it->status()))
//		{
//			fileSink.push_back(it->path().filename().string());
//		}
//	}
//}
//
//void listDirectories(const vector<path>& paths, FileMonitor& fileSink)
//{
//	vector<future<void>> futures;
//
//	for (const auto& path : paths)
//	{
//		cout << path << endl;
//		futures.emplace_back(async(listDirectory, ref(path), ref(fileSink)));
//	}
//
//	for (auto& f : futures)
//	{
//		f.wait();
//	}
//}
//
//int man()
//{
//	vector<path> paths;
//
//	for (directory_iterator it("C:\\"); it != directory_iterator(); ++it)
//	{
//		if (is_directory(it->status()))
//		{
//			paths.push_back(it->path());
//		}
//	}
//
//	FileMonitor fileSink;
//	listDirectories(paths, fileSink);
//
//	return 0;
//}

4 / 17 / 17

5 / 1 / 17 -- > Game Project Presentations
5 / 2 / 17 -- > Prog 2 Exam

concurrency

wait at the end of delivers ?
wait at the end of batch in EventQueue ?

make a test publisher that randomly throws an exception.
make an integer member on construction that is set to rand().
The notify can throw an exception when integer member % 20 = 0 or something.

Make EventSubscriber that enqueues events to test that the EventQueue will not process the newly created events

Add mutex for the Vector inside of EventQueue

Vector of future of void
call get on them so you can trap exceptions from the thread
use std::vector instead of Vector for this

recursive mutex inside of EventQueue

acquire lock
move vector of publishers
release lock
iterate over vector

lock_guard<recursive_mutex> lock(mMutex);

always lock mutexes in the same order

use adopt_lock when you lock_guard construct if you are unsure of the order they are acquired
use std::lock prior to constructing lock_guards with adopt_lock

recursive_mutex for when you may be attempting to acquire the same lock in the same thread

4 / 18 / 17

virtual static mutex method for EventPublisher
Event<T> contains it and returns it in its implementation of that method

also test for an event that unsubscribes
add a static method to Event<T> that returns the size of the subscriber list to confirm

use as in your update calls in world

prediction: 91 - 96

lock mutex before spining threadds
and then unlock after threads exist

try to make use of std::partition
lock mQueue
partition the EventQueue by IsExpired
copy the expired events into another vector
erase the expired events from mQueue
unlock mQueue
iterate over the expired events vector to spin up threads

calling clear during EventQueue::Update destroys publishers that we are iterating over in the copied vector

4 / 19 / 17

C++ thread agregate exception

lock until spun up threads in event publisher - or just do what you have been doing

#include <functional>
#include <map>

//int main()
//{
//	map<string, function<void(int)>> actionMap =
//	{
//		{ "jump", [](int value) { cout << "Jump: " << value <<  << " meters high!" << endl; }},
//		{ "run", [](int value) { cout << "Run: " << value << << " meters far!" << endl; } },
//		{ "dive", [](int value) { cout << "Dive: " << value << << " meters down!" << endl; } },
//	};
//
//	actionMap["jump"](100);
//	actionMap["run"](-100);
//	actionMap["dive"](100000);
//
//	int value = 0;
//
//	auto expression = [](int& x)
//	{
//		cout << ++x << endl;
//	};
//
//	cout << value << endl;
//	expression(value);
//	cout << value << endl;
//
//	return 0;
//}
//
