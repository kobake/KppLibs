#pragma once

#include <boost/shared_ptr.hpp>
template <class T>
class shared_shell_ptr : public boost::shared_ptr<T>{
private:
	typedef boost::shared_ptr<T> Super;
protected:
	class CDeleter{
	public:
		void operator()(T* t)
		{
			if(t){
				t->Release();
			}
		}
	};
public:
	shared_shell_ptr() : Super() { }
	template<class Y> explicit shared_shell_ptr( Y * p ) : Super(p, CDeleter()) { }
};
