#pragma once

class StdError{
public:
	StdError();
	void show_error() const;
	void throw_error() const;
	const wchar_t* to_string() const;
private:
	int e;
};
