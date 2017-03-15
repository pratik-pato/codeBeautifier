
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Choice.H>
#include <cstdlib>                   //for exit(0)
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>
using namespace std;

void copy_cb( Fl_Widget* , void* );  //function prototypes
void close_cb( Fl_Widget* , void* );
const char* beautifyJson(char *,int);
void make_window();
std::string formattedJson(char *,int);
  
int main() {

   make_window();
   return Fl::run();
}


void make_window() {
 
   Fl_Window* win= new Fl_Window(1080,620, "Code Beautifier v 0.1");
   win->begin();      
      Fl_Button*  copy = new Fl_Button( 110, 30, 70, 30, "Beautify");
      Fl_Button* close = new Fl_Button(200, 30, 70, 30, "&Quit");
      Fl_Input*  inp = new Fl_Multiline_Input(30,100, 400, 500, "IN"); 
      Fl_Output*  out = new Fl_Multiline_Output(500,100, 400, 500, "Out");
      Fl_Input*  tab = new Fl_Input(600,30, 70, 30, "Tab Width"); 
      Fl_Choice *language = new Fl_Choice(800,30,130,25,"Language");
      language->add("json");
      language->add("HTML");
      language->add("XML");
      language->add("C");
      language->add("JAVA");
      language->add("Haskell");
      language->value(0);
   win->end();
   copy->callback(copy_cb);
   close->callback(close_cb);
   win->show();
 }


void copy_cb( Fl_Widget* o , void* ) {

   Fl_Button* b=(Fl_Button*)o;
   Fl_Input* iw = (Fl_Input*) b -> parent() -> child(2);
   Fl_Input* itbwdth = (Fl_Input*) b -> parent() -> child(4);
   //printf("%s\n", iw->value());
   char inp[10000];// = (char*)malloc(sizeof(char)*10000);
   strcpy(inp,iw->value());
   //printf("%s\n", inp);
   Fl_Output* ow = (Fl_Output*) b -> parent() -> child(3);
   //ow->value( iw->value() );
   stringstream strValue;
   strValue << itbwdth->value();

   unsigned int tb;
   strValue >> tb;

   ow->value( beautifyJson(inp,tb));
}


void close_cb( Fl_Widget* o, void*) {

   exit(0);
}


std::string generateTab(int n)
{
  std::string tab="";
    for (int i = 0; i < n; i++)
      tab.append(" ");
    return tab;
}

std::string tabStr(int n,int tabWidth)
{
  std::string tab=generateTab(tabWidth);
    for (int i = 0; i < n; i++)
      tab.append(tab);
    return tab;
}


std::string formattedJson(char *json,int tabWidth)
{
    std::string pretty;

    if (json == NULL || strlen(json) == 0)
    {
        return pretty;
    }

    std::string str = std::string(json);
    bool quoted  = false;
    bool escaped = false;
    //std::string INDENT  = generateTab(tabWidth);
    std::string INDENT  = "    ";
    int indent  = 0;
    int length  = (int) str.length();
    int i;

    for (i = 0 ; i < length ; i++)
    {
      char ch = str[i];
      switch (ch)
      {
        case '{':
        case '[':
          pretty += ch;
          if (!quoted)
            {
              pretty += "\n";
              if (!(str[i+1] == '}' || str[i+1] == ']'))
                {
                  ++indent;
                  for (int j = 0 ; j < indent ; j++)
                    {
                      pretty += INDENT;
                    }
                }
            }
          break;

        case '}':
        case ']':
          if (!quoted)
          {
            if ((i > 0) && (!(str[i-1] == '{' || str[i-1] == '[')))
              {
                pretty += "\n";
                --indent;
                for (int j = 0 ; j < indent ; j++)
                  {
                    pretty += INDENT;
                  }
              }
            else if ((i > 0) && ((str[i-1] == '[' && ch == ']') || (str[i-1] == '{' && ch == '}')))
              {
                for (int j = 0 ; j < indent ; j++)
                  {
                    pretty += INDENT;
                  }
              }
          }
          pretty += ch;
          break;

        case '"':
          pretty += ch;
          escaped = false;
            if (i > 0 && str[i-1] == '\\')
            {
              escaped = !escaped;
            }
            if (!escaped)
              {
                quoted = !quoted;
              }
          break;

        case ',':
          pretty += ch;
          if (!quoted)
            {
              pretty += "\n";
                for (int j = 0 ; j < indent ; j++)
                  {
                    pretty += INDENT;
                  }
            }
          break;

        case ':':
          pretty += ch;
          if (!quoted)
            {
              pretty += " ";
            }
          break;

        default:
          pretty += ch;
          break;
      }
    }
    return pretty;
}


const char* beautifyJson(char *inp,int tabWidth) {
  std::string ans=formattedJson(inp,tabWidth);
  const char* beautyJson = ans.c_str();
  return beautyJson;
}