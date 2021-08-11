#include "plugins/LuaWrap.h"

// For compilation
#include "extern/lua-5.4.2/src/lauxlib.h"
#include "extern/lua-5.4.2/src/lbaselib.h"
#include "extern/lua-5.4.2/src/loadlib.h"
#include "extern/lua-5.4.2/src/lcorolib.h"
#include "extern/lua-5.4.2/src/liolib.h"
#include "extern/lua-5.4.2/src/ltablib.h"
#include "extern/lua-5.4.2/src/lstrlib.h"
#include "extern/lua-5.4.2/src/lmathlib.h"
#include "extern/lua-5.4.2/src/lutf8lib.h"
#include "extern/lua-5.4.2/src/ldblib.h"

//}


#include <stdlib.h>
#include <stdio.h>

LuaState::LuaState()
{
  int sum;
  int status;


  /* initialize Lua */
  L = luaL_newstate();

  
  /* load Lua base libraries */
  luaL_openlibs(L);

}

LuaState::~LuaState()
{
  lua_close(L);
}

void LuaState::LoadBuffer(const string & script)
{
  int status = luaL_loadbuffer(L,script.c_str(),script.length(),"=noname");
  if (status) {
    /* If something went wrong, error message is at the top of */
    /* the stack */
    //fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
    cout << "ERROR: Could not load lua script " << endl;
    exit(1);
  }
  
  lua_pcall(L, 0, 0, 0);

}

void LuaState::Load(const string & fileName)
{
  int status = luaL_loadfile(L, fileName.c_str());
  if (status) {
    /* If something went wrong, error message is at the top of */
    /* the stack */
    fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
    cout << "ERROR: Could not read file " << lua_tostring(L, -1) << endl;
    exit(1);
  }
  
  lua_pcall(L, 0, 0, 0);

}


bool LuaState::Call(svec<double> & out, const svec<double> & in, const string & label, const string & func)
{
  int status;
  int i;
  
  /* the function name */
  status = lua_getglobal(L, func.c_str());
  
  //cout << status << endl;
  
  for (i=0; i<in.isize(); i++) {
    lua_pushnumber(L, in[i]);
  }
  lua_pushstring(L, label.c_str());
  
  lua_call(L, in.isize()+1, out.isize());
  
  /* get the result */

  for (i=-out.isize(); i<0; i++) {
    // cout << "Read from Lua: " << i << endl;
    double v = lua_tonumber(L, i);  
    out[out.isize()+i] = v;    
  }
  
  for (i=-out.isize(); i<0; i++) {
    lua_pop(L, i);        
  }
  
  return true;
}

bool LuaState::CallSimple(double & risk,
			  string & comment,
			  const string & func)
{
  int status;
  int i;
  
  /* the function name */
  status = lua_getglobal(L, func.c_str());
  
  //cout << status << endl;
  
 
  lua_call(L, 0, 2);
  
  /* get the result */

 
  risk = lua_tonumber(L, -2);  
  comment = lua_tostring(L, -1);  
  
  lua_pop(L, -2);        
  lua_pop(L, -1);
  
  return true;
}
