#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <unistd.h>

using namespace std;

using strs = vector<vector<int>>;

class Befunge {
  private:
    const int dx[4] = {1, 0,-1, 0};
    const int dy[4] = {0, 1, 0,-1};

    strs code;
    strs out;
    vector<int> stack;

    int w, h;
    int x, y, d;

    bool lit;

    int px, py;
    int sw, oh;
    int psl;
    int ms;

    mt19937 mt;


    void push(int val);
    int pop();

    int get_code(int i, int j);
    void push_code(int i, int j, int v);

    void set_out(int val);
    void set_out_s(string str);

    bool step();

    void show_char(int v);
    void update_char(int i, int j, int v);

    void show_code();
    void update_show_code();
    void show_stack();
    void update_show_stack();
    void show_out();
    void update_show_out();

    void show();

  public:
    Befunge();

    void push_line(string str);
    void set_delay(int sp);

    void interpreter();

};

Befunge::Befunge(): code(), out(), stack(), w(0), h(0), x(0), y(0), d(0), lit(false), px(0), py(0), sw(0), oh(0), psl(0), ms(10) {
  random_device rnd;
  mt.seed(rnd());

  vector<int> a;
  out.push_back(a);
}



void Befunge::push(int val) {
  stack.push_back(val);
  update_show_stack();
}

int Befunge::pop() {
  if(stack.empty())return 0;
  int val = stack.back();
  stack.pop_back();
  return val;
}


int Befunge::get_code(int i, int j) {
  if(j<(int)code.size() && i<(int)code[j].size())return code[j][i];
  else return (int)' ';
}

void Befunge::push_code(int i, int j, int v) {
  while(!(j<(int)code.size())) {
    vector<int> a;
    code.push_back(a);
  }

  while(!(i<(int)code[j].size())) {
    code[j].push_back((int)' ');
  }

  code[j][i] = v;
  update_char(i, j, v);
}


void Befunge::set_out(int val) {
  if(val-'\n')out[oh].push_back(val);
  else {
    vector<int> a;
    out.push_back(a);
    oh++;
  }
  update_show_out();
}

void Befunge::set_out_s(string str) {
  for(int i=0;i<(int)str.size();i++)set_out( (int)str[i] );
}


bool Befunge::step() {
  x += w+dx[d];
  y += h+dy[d];

  x %= w;
  y %= h;

  return (get_code(x, y) == (int)' ');
}


void Befunge::show_char(int v) {
  cout << (isprint((char)v) ? (char)v : '?');
}

void Befunge::update_char(int i, int j, int v) {
  cout << "\x1b[" << j+1 << ";" << i+1 << "H";
  show_char(v);
}


void Befunge::show_code() {
  for(int i=0;i<(int)code.size();i++) {
    for(int j=0;j<(int)code[i].size();j++) {
      show_char(code[i][j]);
    }
    cout<<'\n';
  }

  px = x;
  py = y;
}

void Befunge::update_show_code() {
  update_char(px, py, get_code(px, py));
  cout << "\x1b[43m";
  update_char(x, y, get_code(x, y));
  cout << "\x1b[0m";

  px = x;
  py = y;
  //cout<<"\x1b["<<h+oh+4<<";0H"<<x<<','<<y;
}

void Befunge::show_stack() {
  for(int i=0;i<(int)stack.size();i++)cout << stack.at(i) << ' ';
  cout<<"\x1b[K\n";
}

void Befunge::update_show_stack() {
  cout<<"\x1b["<<h+1<<";0H";
  show_stack();
}

void Befunge::show_out() {
  for(int i=0;i<=oh;i++) {
    for(int j=0;j<(int)out[i].size();j++) {
      show_char(out[i][j]);
    }
    cout<<'\n';
  }
}

void Befunge::update_show_out() {
  if( out[oh].size() )update_char( (int)out[oh].size()-1, h+oh+1, out[oh].back() );
}


void Befunge::show() {
  show_code();
  show_stack();
  show_out();
}



void Befunge::push_line(string str) {
  vector<int> a;
  for(int i=0;i<(int)str.size();i++)a.push_back( (int)str[i] );
  code.push_back(a);
  w = max(w, (int)str.size());
  h++;
}

void Befunge::set_delay(int sp) {
  ms = sp;
}


void Befunge::interpreter() {
  int a, b;
  char e;

  cout<<"\x1b[2J\x1b[0;0H";
  show();
  while(true) {
    update_show_code();

    if(lit) {
      if(get_code(x, y) == (int)'"')lit = false;
      else push(get_code(x, y));

    }else {
      switch(get_code(x, y)) {
        case '>':
          d = 0;
          break;
        case 'v':
          d = 1;
          break;
        case '<':
          d = 2;
          break;
        case '^':
          d = 3;
          break;
        case '_':
          a = pop();
          if(a)d = 2;
          else d = 0;
          break;
        case '|':
          a = pop();
          if(a)d = 3;
          else d = 1;
          break;
        case '?':
          d = mt()%4;
          break;
        case '#':
          step();
          break;
        case '@':
          cout<<"\x1b["<<h+oh+4<<";0H";
          return;
          break;
        case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
          push(get_code(x, y)-'0');
          break;
        case '"':
          lit = true;
          break;
        case '&':
          cout<<"\x1b["<<h+oh+3<<";0H";
          for(cin>>a;!cin;cin>>a) {
            cin.clear();
            cin.ignore();
          }
          push(a);
          break;
        case '~':
          cout<<"\x1b["<<h+oh+3<<";0H";
          for(cin>>e;!cin;cin>>e) {
            cin.clear();
            cin.ignore();
          }
          push((int)e);
          break;
        case '.':
          a = pop();
          set_out_s(to_string(a));
          set_out(' ');
          break;
        case ',':
          a = pop();
          set_out(a);
          break;
        case '+':
          b = pop();
          a = pop();
          push(a+b);
          break;
        case '-':
          b = pop();
          a = pop();
          push(a-b);
          break;
        case '*':
          b = pop();
          a = pop();
          push(a*b);
          break;
        case '/':
          b = pop();
          a = pop();
          push(b?0:a/b);
          break;
        case '%':
          b = pop();
          a = pop();
          push(a%b);
          break;
        case '`':
          b = pop();
          a = pop();
          push(a>b);
          break;
        case '!':
          a = pop();
          push(!a);
          break;
        case ':':
          a = pop();
          push(a);
          push(a);
          break;
        case '\\':
          b = pop();
          a = pop();
          push(b);
          push(a);
          break;
        case '$':
          a = pop();
          break;
        case 'g':
          b = pop();
          a = pop();
          push( get_code(a, b) );
          break;
        case 'p':
          b = pop();
          a = pop();
          push_code( a, b, pop() );
          break;
      }

    }

    a = x;
    b = y;
    if(lit)step();
    else while(step()) {
      if(a==x && b==y)return ;
    }

    usleep(ms*1000);

  }

}



Befunge befunge;

int main(int argc, char** argv) {
  ios::sync_with_stdio(false);

  string filename;
  for(int i=1;i<argc;i++) {
    if(!string(argv[i]).find("-delay")) {
      if(++i<argc)befunge.set_delay(stoi(argv[i]));
    }else filename = argv[i];
  }

  ifstream ifs(filename);

  if (ifs.fail()) {
    cerr<<"\x1b[1m\x1b[31m"<<"fatal error:"<<"\x1b[0m"<<" no input files"<<endl;
    return -1;
  }

  string s;
  while(getline(ifs, s))befunge.push_line(s);

  befunge.interpreter();

  return 0;
}

