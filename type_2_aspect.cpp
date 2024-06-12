
#include "mixutil.h"


template < class T >
struct Foo
{
	using fooType = typename type_aspect_if<typename T, mix::ptrTraits<T>::isPointer>::type;

	Foo() {};
	
	Foo(const fooType uv)
	{
		_mData = uv;
	}

	~Foo()
	{
		if (mix::ptrTraits<fooType>::isPointer)
		{
			_mData = NULL;

		}
	}

	fooType get() const {
		return _mData;
	}

	void set(const fooType _uv)
	{
		if (mix::ptrTraits<fooType>::isPointer)
			_mData = _uv;
		else
			_mData = fooType(_uv);

	}

private:
	fooType _mData;
};


int main(int argc, const char* argv[]) 
{
	
	int nc = 625;
	Foo<int> fu;
	Foo<int*> fup;

	fu.set(220);
	fup.set(&nc);
	

	PRINT(fu.get());
	PRINT(*fup.get());

	RET2();

	return -1;
}
