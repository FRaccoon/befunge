#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

class Befunge {
private:
  const int dx[4] = {1, 0,-1, 0};
  const int dy[4] = {0, 1, 0,-1};
  
  char c[25][80];
  int w, h;
  int x, y, d;
  bool skip, code;
  stack<int> st;
  string o;
  
  mt19937 mt;
  
public:
  Befunge();
  void set_line(int i, string s);
  void push_stack(int i);
  int pop_stack();
  void interpreter();
  void show();
  void erase_show();
  void output(string s);
  
};

Befunge::Befunge(): w(0), h(0), x(0), y(0), d(0), skip(false), code(false) {
  random_device rnd;
  mt.seed(rnd());
}

void Befunge::set_line(int i, string s) {
  h = max(h, i%25+1);
  w = min(80, max(w, (int)s.size()));
  for(int j=0;j<w;j++)c[i][j] = s[j];
}

void Befunge::push_stack(int i) {
  st.push(i);
}

int Befunge::pop_stack() {
  int s = st.top();
  st.pop();
  return s;
}

void Befunge::interpreter() {
  show();
  if(skip) skip = false;
  else if(code) {
    if(c[y][x]=='"')code = false;
    else push_stack((int)c[y][x]);
  }else {
    int a, b;
    char e;
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
      case '#':skip = true;break;
      case '@':return;break;
      case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
        push_stack(c[y][x]-'0');
      break;
      case '"':code = true;break;
      case '&':
        cin>>a;
        push_stack(a);
      break;
      case '~':
        cin>>e;
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
        e = (char)pop_stack();
        c[b][a] = e;
      break;
    }
  }
  
  x += w+dx[d];
  y += h+dy[d];
  x %= w;
  y %= h;
  
  usleep(50000);
  erase_show();
  
  interpreter();
}

void Befunge::show() {
  for(int i=0;i<h;i++) {
    for(int j=0;j<w;j++) {
      if(i==y&&j==x)cout<<"\x1b[43m";
      cout<<c[i][j];
      if(i==y&&j==x)cout<<"\x1b[0m";
    }
    cout<<endl;
  }
  cout<<o<<endl;
}

void Befunge::erase_show() {
  cout<<"\x1b["+to_string(h+1)+"A";
}

void Befunge::output(string s) {
  o += s;
}

Befunge befunge;

int main() {
  
  string s;
  for(int i=0;i<25&&getline(cin, s);i++)befunge.set_line(i, s);
  
  befunge.interpreter();
  //befunge.show();
}
