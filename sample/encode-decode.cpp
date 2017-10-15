#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

#define INFILE "tes.txt"
#define OUTFILE "out.txt"

string convertToByte(char c){
  int oct = int(c);
  char s[8];
  vector<int> v;
  while(oct){
    v.push_back(oct%2);
    oct/=2;
  }
  string ret="";
  int diff = 8-v.size();

  for (int i=0; i<diff; i++){
    ret+="0";
  }
  for (int i = diff; i < 8; i++){
    ret+= (v[v.size()-1-i+diff] == 1)? '1' : '0';
  }

  return ret;
}

vector<string> readFile(){
  static vector<string> v;

  std::filebuf fb;
  if (fb.open (INFILE,std::ios::in))
  {
    std::istream is(&fb);
    while (is){
      string ss = convertToByte(char(is.get()));
      v.push_back(ss);
    }
    fb.close();
  }

  /* Per line
  string line;
  ifstream myfile ("tes.txt");
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      for (int i=0; i < line.length();i++){
        string ss = convertToByte(line[i]);
        //cout << ss << "  ";
        v.push_back(ss);
      }
    }
    myfile.close();
  }*/

  else {
    cout << "Unable to open file";
  }
  return v;

}

char ByteToChar(string s){
  int num=0,bit;
  for (int i = 0; i < 8; i++){
    bit = (s[i] == '0') ? 0 : 1;
    num+= bit*pow(2,8-1-i);
  }
  return char(num);
}

void writeFile(vector<string> v){
  std::filebuf fb;
  fb.open (OUTFILE,std::ios::out);
  std::ostream os(&fb);
  for (int i = 0; i < v.size(); i++){
    os << ByteToChar(v[i]);
  }
  fb.close();
}

int main(){
  vector<string> v = readFile(); // Read from file
  v.pop_back(); v.pop_back(); //delete 2 weird character

  for(int i = 0; i < v.size(); i++){
    cout << i << ":" << v[i] << endl;
  }
  /*cout << "Result : \n";
  for (int i= 0; i < v.size(); i++)
    cout << "\t"<<v[i] << endl;*/

  writeFile(v);// Write to file
  return 0;
}
