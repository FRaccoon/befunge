#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

class Befunge {
private:
  const int dx[4] = {1, 0,-1, 0};
  const int dy[4] = {0, 1, 0,-1};
  
  vector<vector<int>> c;
  int w, h;
  int x, y, d;
  bool code;
  vector<int> stack;
  string o;
  int ms;
  
  mt19937 mt;
  
public:
  Befunge();
  void push_line(string s);
  void fill_space();
  void push_stack(int i);
  int pop_stack();
  void interpreter();
  void show();
  void erase_show();
  void output(string s);
  void set_delay(int s);
  bool step();
  
};

Befunge::Befunge(): w(0), h(0), x(0), y(0), d(0), code(false), stack(), o(), ms(50) {
  random_device rnd;
  mt.seed(rnd());
}

void Befunge::push_line(string s) {
  vector<int> a;
  for(int i=0;i<(int)s.size();i++)a.push_back( (int)s[i] );
  c.push_back(a);
  w = max(w, (int)s.size());
  h++;
}

void Befunge::fill_space() {
  for(int i=0;i<(int)c.size();i++) {
    for(int j=(int)c[i].size();j<w;j++)c[i].push_back( (int)' ' );
  }
}

void Befunge::push_stack(int i) {
  stack.push_back(i);
}

int Befunge::pop_stack() {
  int s = stack.back();
  stack.pop_back();
  return s;
}

void Befunge::interpreter() {
  show();
  
  int a, b;
  char e;
  
  if(code) {
    if(c[y][x] == '"')code = false;
    else push_stack((int)c[y][x]);
    
  }else {
    switch(c[y][x]) {
      case '>':d = 0;break;
      case 'v':d = 1;break;
      case '<':d = 2;break;
      case '^':d = 3;break;
      case '_':
        a = pop_stack();
        if(a)d = 2;
        else d = 0;
      break;
      case '|':
        a = pop_stack();
        if(a)d = 3;
        else d = 1;
      break;
      case '?':d = mt()%4;break;
      case ' ':break;
      case '#':step();break;
      case '@':return;break;
      case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
        push_stack(c[y][x]-'0');
      break;
      case '"':code = true;break;
      case '&':
        for(cin>>a;!cin;cin>>a) {
          cin.clear();
          cin.ignore();
        }
        push_stack(a);
      break;
      case '~':
        for(cin>>e;!cin;cin>>e) {
          cin.clear();
          cin.ignore();
        }
        push_stack((int)e);
      break;
      case '.':
        a = pop_stack();
        output(to_string(a));
        output(" ");
      break;
      case ',':
        a = pop_stack();
        output(""s+(char)a);
      break;
      case '+':
        b = pop_stack();
        a = pop_stack();
        push_stack(a+b);
      break;
      case '-':
        b = pop_stack();
        a = pop_stack();
        push_stack(a-b);
      break;
      case '*':
        b = pop_stack();
        a = pop_stack();
        push_stack(a*b);
      break;
      case '/':
        b = pop_stack();
        a = pop_stack();
        push_stack(a/b);
      break;
      case '%':
        b = pop_stack();
        a = pop_stack();
        push_stack(a%b);
      break;
      case '`':
        b = pop_stack();
        a = pop_stack();
        push_stack(a>b);
      break;
      case '!':
        a = pop_stack();
        push_stack(!a);
      break;
      case ':':
        a = pop_stack();
        push_stack(a);
        push_stack(a);
      break;
      case '\\':
        b = pop_stack();
        a = pop_stack();
        push_stack(b);
        push_stack(a);
      break;
      case '$':
        a = pop_stack();
      break;
      case 'g':
        b = pop_stack();
        a = pop_stack();
        push_stack(c[b][a]);
      break;
      case 'p':
        b = pop_stack();
        a = pop_stack();
        c[b][a] = pop_stack();
      break;
    }
    
  }
  
  a = x;
  b = y;
  if(code)step();
  else while(step()) {
    if(a==x && b==y)return ;
  }
  
  usleep(ms*1000);
  erase_show();
  
  interpreter();
}

void Befunge::show() {
  for(int i=0;i<h;i++) {
    for(int j=0;j<w;j++) {
      if(i==y&&j==x)cout<<"\x1b[43m";
      if(isprint((char)c[i][j]))cout<<(char)c[i][j];
      else cout<<'?';
      if(i==y&&j==x)cout<<"\x1b[0m";
    }
    cout<<endl;
  }
  for(int i=0;i<(int)stack.size();i++)cout<<stack.at(i)<<' ';
  cout<<endl<<o<<endl;
}

void Befunge::erase_show() {
  system("clear");
  //cout<<"\x1b["+to_string(h+2)+"A";
}

void Befunge::output(string s) {
  o += s;
}

void Befunge::set_delay(int s) {
  ms = s;
}

bool Befunge::step() {
  x += w+dx[d];
  y += h+dy[d];
  x %= w;
  y %= h;
  return c[y][x] == ' ';
}

Befunge befunge;

int main(int argc, char** argv) {
  
  string filename;
  for(int i=1;i<argc;i++) {
    if(!strcmp(argv[i], "-delay")) {
      if(++i<argc) befunge.set_delay(stoi(argv[i]));
    }else filename = argv[i];
  }
  
  ifstream ifs(filename);
  
  if (ifs.fail()) {
    cerr<<"\x1b[1m\x1b[31m"<<"fatal error:"<<"\x1b[0m"<<" no input files"<<endl;
    return -1;
  }
   
  string s;
  while(getline(ifs, s))befunge.push_line(s);
  befunge.fill_space();
  
  befunge.interpreter();
  
  return 0;
}

