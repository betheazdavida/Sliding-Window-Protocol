#include <iostream>
#include <fstream>

using namespace std;


void readFile(){

  std::filebuf fb;
  if (fb.open ("tes.txt",std::ios::in))
  {
    std::istream is(&fb);
    while (is)
      std::cout << char(is.get());
    fb.close();
  }

}

int main(){
  readFile();
  return 0;
}
