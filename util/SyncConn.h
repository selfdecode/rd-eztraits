#ifndef SYNC_CONN_H
#define SYNC_CONN_H

#include "util/SComm.h"
#include <string>
#include "base/FileParser.h"


class SyncConnServer
{
 public:
  // NOTE: The port is the base, it will use up to 4 beyond that.
  SyncConnServer(const string & server, int port = MYPORT);
  ~SyncConnServer();

  bool WaitForRequest(string & request);
  bool SendResult(const string & s);

 private:
  SCommTransmitter * m_pTrans;
  SCommReceiver * m_pRec;
  string m_server;
  int m_port;
  char * m_pData;
};

class SyncConnClient
{
 public:
  // NOTE: The port is the base, it will use up to 4 beyond that.
  SyncConnClient(const string & server, int port = MYPORT);
  ~SyncConnClient();

  bool SendRequest(string & result, const string & request);

 private:
  SCommTransmitter * m_pTrans;
  SCommReceiver * m_pRec;
  string m_server;
  int m_port;
  char * m_pData;
};

#endif //SYNC_CONN_H



