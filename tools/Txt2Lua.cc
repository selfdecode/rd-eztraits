#include <string>
#include "base/CommandLineParser.h"
#include "base/FileParser.h"


bool Any(const string & s)
{
  if (s == "any")
    return true;
  if (s == "any:")
    return true;
  if (s == "Any")
    return true;
  if (s == "Any:")
    return true;

  return false;
}

bool All(const string & s)
{
  if (s == "all")
    return true;
  if (s == "all:")
    return true;
  if (s == "All")
    return true;
  if (s == "All:")
    return true;
  if (s == "if")
    return true;
  if (s == "if:")
    return true;
  if (s == "If")
    return true;
  if (s == "If:")
    return true;

  return false;
}

string Clean(const string & s) {
  int i;
  string out = s;

  
  
  int rep = 0;
  int delim = 0;
  for (i=0; i<(int)out.length(); i++) {
    if (out[i] == '\'') {
      out[i] = '"';
      rep++;
    }
    if (out[i] == '/' || out[i] == '|')
      delim++;
  }

  if (rep == 0)
    out = "\"" + s + "\"";

  if (delim == 0) {
    string o;
    o += out[0]; 
    o += out[1]; 
    o += "/"; 
    o += out[2]; 
    o += out[3];

    out = o;
  }
  
  return out;
}


void LineAny(svec<string> & out, FlatFileParser & parser)
{
  int i;
  string s = "        if " + parser.AsString(0) + " == ";

  for (i=1; i<parser.GetItemCount(); i++) {
    string a = s + Clean(parser.AsString(i)) + " then";
    out.push_back(a);
  }
  
}

void LineAll(string & out, FlatFileParser & parser, int off = 0)
{
  int i;
  
  string s;

  if (out == "")
    s = "if (" + parser.AsString(off) + " == ";
  else
    s = " and (" + parser.AsString(off) + " == ";

  string o = " or " + parser.AsString(off) + " == ";

  bool bOr = false;
  for (i=1+off; i<parser.GetItemCount(); i++) {
    if (parser.AsString(i) == "or") {
      bOr = true;
      continue;
    }
    string a;
    if (i == 1)
      a = s + Clean(parser.AsString(i));
    else
      a = o + Clean(parser.AsString(i));
    out += a;
  }
  out += ")";
  
}


int main( int argc, char** argv )
{

  commandArg<string> fileCmmd("-i","input file");
  commandLineParser P(argc,argv);
  P.SetDescription("Translates text to lua snippet.");
  P.registerArg(fileCmmd);
 
  P.parse();
  
  string fileName = P.GetStringValueFor(fileCmmd);
 

  //comment. ???
  FlatFileParser parser;
  
  parser.Open(fileName);

  cout << "        -- This scriptlet was automatically" << endl;
  cout << "        -- generated from" << endl;
  cout << "        -- " << fileName << endl << endl;


  bool bAny = false;
  bool bAll = false;

  svec<string> any;
  string all;

  int i, j;
  
  while (parser.ParseLine()) {
  
    if (parser.GetItemCount() == 0)
      continue;
    if (Any(parser.AsString(0))) {
      bAny = true;
      bAll = false;
      any.clear();
      continue;
    }
    if (All(parser.AsString(0))) {
      bAny = false;
      bAll = true;
      all = "";
      if (parser.GetItemCount() > 1) {
	//LineAll(all, parser, 1);
      }
      continue;
    }

    if (parser.AsString(0) == "else") {
      cout << "        comment =";
      for (i=1; i<parser.GetItemCount(); i++)
	cout << " " << parser.AsString(i);
      cout << endl;
      continue;
    }

    if (parser.AsString(0) == "=" || parser.AsString(0) == "==") {
      if (bAll) {
	cout << "        " + all + " then" << endl;
	cout << "          comment = ";
	for (i=1; i<parser.GetItemCount(); i++)
	  cout << " " << parser.AsString(i);
	cout << endl;
	cout << "          return risk, comment" << endl;
	cout << "        end" << endl << endl;
	bAll = false;
      }

      if (bAny) {
	for (i=0; i<any.isize(); i++) {
	  cout << any[i] << endl;
	  cout << "          comment = ";
	  for (j=1; j<parser.GetItemCount(); j++)
	    cout << " " << parser.AsString(j);
	  cout << endl;
	  cout << "          return risk, comment" << endl;
	  cout << "        end" << endl << endl;
	  
	}

	bAny = false;
      }
      
      any.clear();
      all = "";
    }

    
    if (bAny)
      LineAny(any, parser);
    if (bAll)
      LineAll(all, parser);
    
  }
  return 0;
}
