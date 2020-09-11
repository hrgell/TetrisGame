#include "stdafx.h"
#include "Scope.h"
#include "Util.h"

using namespace std;

//
// Scope(function<bool()> initializer, function<void()> finalizer, const char *msg)
//
// The first two arguments initializer and finalizer corresponds to a constructor and a destructor.
// The initializer takes no arguments and must return true on success and false on failure.
// The finalizer takes no arguments and returns no value.
// The thrird argument is an error message that is thrown if the initialier returns false.
//
// The initializer is called immediately when the instance is created.
// If the initializer returned true then the finalizer will be called otherwise it wont.
// The finalizer is calleded when the instance is destroyed.
// If the error message is not empty then an exception is thrown containing the message.
//
// Example:
//
//     char *buf;
//     Scope scope1(
//         [&]() { buf = static_cast<char *>(malloc(BUFSIZ)); return (buf != NULL); },
//         [&]() { free(buf); },
//         "Failed to allocated memory");
//     strncpy(buf, "Hello", 5);

// The finalizer may be called and an exception may be thrown.
Scope::Scope(function<bool()> initializer, function<void()> finalizer, const char *msg)
	: initialized(initializer()), finalizer(finalizer)
{
	if (!initialized && msg != NULL && msg[0] != '\0')
		throw std::runtime_error(msg);
}

// No finalizer is called and an exception may be thrown.
Scope::Scope(function<bool()> initializer, const char *msg)
	: initialized(false)
{
	if (!initializer() && msg != NULL && msg[0] != '\0')
		throw std::runtime_error(msg);
}

// The finalizer is always called.
Scope::Scope(function<void()> initializer, function<void()> finalizer)
	: initialized(true), finalizer(finalizer)
{
	initializer();
}

// No finalizer is called.
Scope::Scope(function<void()> initializer)
	: initialized(false)
{
	initializer();
}

Scope::Scope(Scope &&scope) :
	initialized(std::move(scope.initialized)),
	finalizer(std::move(scope.finalizer))
{
	scope.initialized = false;
}

Scope& Scope::operator=(Scope&& other) {
	initialized = std::move(other.initialized);
	finalizer = std::move(other.finalizer);
	other.initialized = false;
	return *this;
}

Scope::~Scope()
{
	//DebugWriteMsg("~Scope");
	if (initialized) {
		initialized = false;
		finalizer();
	}
}