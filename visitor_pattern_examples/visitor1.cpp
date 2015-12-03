#include <iostream>
#include <functional>

struct MyStruct
{
  MyStruct() : state{1}
  {
  }

  MyStruct(std::function<void(MyStruct&)> v)
  {
    v(*this);
  }

  int state;
};

int main()
{
  const MyStruct s1;

  const MyStruct s2([](MyStruct&  s) {
      s.state = 2;
    });

  const MyStruct s3 = MyStruct([](MyStruct&  s) {
      s.state = 3;
    });

  std::cout << "s1: " << s1.state << std::endl;
  std::cout << "s2: " << s2.state << std::endl;
  std::cout << "s3: " << s3.state << std::endl;
}
