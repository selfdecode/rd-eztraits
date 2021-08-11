#ifndef LUAWRAP_H
#define LUAWRAP_H

#include "base/SVector.h"
#include <string>

#include <stdio.h>

#include "extern/lua-5.4.2/src/lua.h"
#include "extern/lua-5.4.2/src/linit.h"
#include "extern/lua-5.4.2/src/lualib.h"


class LuaState
{
 public:
  LuaState();
  
  ~LuaState();

  void Load(const string & fileName);

  void LoadBuffer(const string & code);

  // Out.isize has the number of return variables!
  bool Call(svec<double> & out,
	    const svec<double> & in,
	    const string & label,
	    const string & func);

  bool CallSimple(double & risk,
		  string & comment,
		  const string & func);
  
 private:
  lua_State *L;

};












#endif




