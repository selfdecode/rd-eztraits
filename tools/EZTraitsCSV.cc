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

  commandArg<string> fileCmmd("-i","input file (csv)");
  commandArg<string> luaCmmd("-lua","raw lua file");
  commandArg<string> rsCmmd("-rs","header for column listing the rs ids", "");
  commandArg<string> posCmmd("-pos","header for column listing the chromosomal position", "");
  commandArg<string> callCmmd("-call","genotype call column header");
  commandArg<bool> printCmmd("-print","print the full lua script", false);
  commandLineParser P(argc,argv);
  P.SetDescription("Calling traits via a lua script.");
  P.registerArg(fileCmmd);
  P.registerArg(luaCmmd);
  P.registerArg(rsCmmd);
  P.registerArg(posCmmd);
  P.registerArg(callCmmd);
  P.registerArg(printCmmd);

  P.parse();
  
  string fileName = P.GetStringValueFor(fileCmmd);
  string rsColName = P.GetStringValueFor(rsCmmd);
  string callColName = P.GetStringValueFor(callCmmd);
  string posColName = P.GetStringValueFor(posCmmd);
  string lua = P.GetStringValueFor(luaCmmd);
  bool bPrint = P.GetBoolValueFor(printCmmd);

  if (rsColName == "" && posColName == "") {
    cout << "ERROR: Please specify either -rs, -pos, or both!!" << endl;
    return -1;
  }

  svec<string> code;
  svec<string> params;
  ParseLua(code, params, lua);

  int i;

  /*
  for (i=0; i<params.isize(); i++) {
    cout << params[i] << endl;
  }

  for (i=0; i<code.isize(); i++) {
    cout << code[i] << endl;
    }*/

  string out = "function evaluate ( )\n\n";
  out += "\tcomment = \"none\"\n";
  out += "\trisk = 0\n\n";
  
  
  //comment. ???
  FlatFileParser parser;
  
  parser.Open(fileName);


  int callCol = -1;
  int posCol = -1;
  int rsCol = -1;

  parser.ParseLine();

  for (i=0; i<parser.GetItemCount(); i++) {
    //cout << parser.AsString(i) << endl;
    if (parser.AsString(i) == rsColName)
      rsCol = i;
    if (parser.AsString(i) == posColName)
      posCol = i;
    if (parser.AsString(i) == callColName)
      callCol = i;

  }
  if (callCol == -1) {
    cout << "NOT FOUND: " << callColName << endl;
    return 0;
  }
  //cout << rsCol << " " << posCol << " " << callCol << endl;
  
  while (parser.ParseLine()) {
    if (parser.GetItemCount() == 0)
      continue;
    
    string call = parser.AsString(callCol);
    string rs;
    if (rsCol != -1)
      rs = parser.AsString(rsCol);
    string pos;
    if (posCol != -1)
      pos = parser.AsString(posCol);
    Replace(pos);
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
