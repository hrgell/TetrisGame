#pragma once
#include <functional>

class Scope
{
private:
	bool initialized;
	std::function<void()> finalizer;
public:
	Scope(std::function<bool()>, std::function<void()>, const char *);
	Scope(std::function<bool()>, const char *);
	Scope(std::function<void()>, std::function<void()>);
	Scope(std::function<void()>);
	Scope() = delete;
	Scope(const Scope &) = delete;
	Scope &operator=(const Scope &) = delete;
	Scope(Scope &&) noexcept;
	Scope& operator=(Scope &&) noexcept;
	~Scope();
	// Remove default constructors and assignment
};
