#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"
#include "plugins/LuaWrap.h"

void Replace(string & s)
{
  for (int i=0; i<(int)s.length(); i++) {
    //cout << s[i] << endl;
    if (s[i] == ':') {
      s[i] = '_';
      //cout << "YESS" << endl;
    }
  }
  //cout << s << endl;
}

void Find(svec<string> & s, const string & key, const string & line)
{
  int i, j;
  int n = (int)key.length();

  //cout << "LINE " << line << endl;
  
  for (i=0; i<(int)line.length()-n; i++) {
    bool yes = true;
    //cout << "------------------" << endl;
    for (j=i; j<i+n; j++) {
      if (line[j] != key[j-i]) {
	yes = false;
	//cout << line[j] << " - " << key[j-i] << endl;
	//break;
      }
    }

    

    if (yes) {
      string t = key;
      
      //cout << "Found key " << key << " pos " << i << endl;
      //cout << i << " " << n << endl;
      for (j=i+n; j<(int)line.length(); j++) {
	//cout << "CHAR " << line[j] << endl;
	if ((line[j] < '0' || line[j] > '9') && line[j] != '_')
	  break;
	t += line[j];
	//cout << t << endl;
      }
      i = j-1;
      //cout << "PUSH " << t << endl;
      s.push_back(t);
    }
  }
}

void ParseLua(svec<string> & code, svec<string> & params, const string & fileName)
{
  //comment. ???
  FlatFileParser parser;
  
  parser.Open(fileName);

  int i;
  
  while (parser.ParseLine()) {
    string l = parser.Line();
    Replace(l);
    //cout << l << endl;
    code.push_back(l);

    Find(params, "rs", l);
    Find(params, "chr", l);
    
  }

  UniqueSort(params);
}

int main( int argc, char** argv )
{

  commandArg<string> fileCmmd("-i","input VCF file");
  commandArg<string> luaCmmd("-lua","raw lua file");
  commandArg<bool> printCmmd("-print","print the full lua script", false);
  commandLineParser P(argc,argv);
  P.SetDescription("Calling traits via a lua script.");
  P.registerArg(fileCmmd);
  P.registerArg(luaCmmd);
  P.registerArg(printCmmd);

  P.parse();
  
  string fileName = P.GetStringValueFor(fileCmmd);
  string lua = P.GetStringValueFor(luaCmmd);
  bool bPrint = P.GetBoolValueFor(printCmmd);

  svec<string> code;
  svec<string> params;
  ParseLua(code, params, lua);

  int i;

 
  string out = "function evaluate ( )\n\n";
  out += "\tcomment = \"none\"\n";
  out += "\trisk = 0\n\n";
  
  
  //comment. ???
  FlatFileParser parser;
  
  parser.Open(fileName);


  
  while (parser.ParseLine()) {
    if (parser.GetItemCount() == 0)
      continue;

    if (parser.Line()[0] == '#')
      continue;
    
    //string call = parser.AsString(callCol);
    string rs;
   
    rs = parser.AsString(2);
    string pos = parser.AsString(0) + "_" + parser.AsString(1);

    string ref = parser.AsString(3);
    string alt = parser.AsString(4);


    //StringParser p;
    //p.SetLine(parser.AsString(9), ":");
    string s = parser.AsString(9);
    string call;
    
    if (s[0] == '0')
      call = ref + "/";
    else
      call = alt + "/";

    if (s[2] == '0')
      call += ref;
    else
      call += alt;
     
    for (i=0; i<params.isize(); i++) {
      if (rs == params[i]) {
	out += "\t" + rs + " = \"" + call + "\"\n";
      }
      if (pos == params[i]) {
	out += "\t" + pos + " = \"" + call + "\"\n";
      }
    }
  }

  for (i=0; i<code.isize(); i++)
    out += code[i] + "\n";
  
  out += "\treturn risk, comment\n";
  out += "end\n";

  if (bPrint) {
    cout << "<SCRIPT> ------------------------------------------" << endl;
    
    cout << out << endl;
    cout << "------------------------------------------</SCRIPT>" << endl << endl;
  }

  
  LuaState lstate;

  lstate.LoadBuffer(out);
  double risk = 0.;
  string comment;
  
  lstate.CallSimple(risk, comment, "evaluate");
  cout << "Risk:    " << risk << endl;
  cout << "Comment: " << comment << endl;
  return 0;
}
