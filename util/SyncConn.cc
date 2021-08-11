#include "util/SyncConn.h"
#include <unistd.h>

// This is STUPID!!!
#define MAXDATASIZE 1048576


SyncConnServer::SyncConnServer(const string & server, int port)
{
  m_server = server;
  m_pTrans = NULL;
  m_pRec = NULL;
  m_port = port;
  //m_pTrans = GetTransmitter(m_port+1);
  //m_pRec = GetReceiver(server.c_str(), m_port+2);
  m_pData = new char[MAXDATASIZE];
}

SyncConnServer::~SyncConnServer()
{
  if (m_pTrans != NULL)
    delete m_pTrans;
  if (m_pRec != NULL)
    delete m_pRec;
  delete [] m_pData;
}

bool SyncConnServer::WaitForRequest(string & request)
{
  m_pTrans = GetTransmitter(m_port+1);
  char data[1024];
  strcpy(data, "Server waiting");
  //cout << "Waiting." << endl;
  m_pTrans->SendWait(data);
  //cout << "Message was picked up." << endl;
  delete m_pTrans;
  m_pTrans = NULL;

  int i;

  //Got something... let's return.
  m_pRec = GetReceiver(m_server.c_str(), m_port+2);
  //char tmp[2048];
  int max = MAXDATASIZE;
  for (i=0; i<50; i++) {
    if (m_pRec->Get(m_pData, max)) {
      //cout << "Got request: " << tmp << endl;
      break;
    } else {
      //cout << "Receive error!" << endl;
      usleep(100000);
    }
  }

  request = m_pData;

  delete m_pRec;
  m_pRec = NULL;
  return true;
}

bool SyncConnServer::SendResult(const string & s)
{
  //cout << "Sending result." << endl;
  m_pTrans = GetTransmitter(m_port+1);
  m_pTrans->SendWait(s.c_str());
  //cout << "Done." << endl;

  delete m_pTrans;
  m_pTrans = NULL;
  return true;
}

//========================================================
SyncConnClient::SyncConnClient(const string & server, int port)
{
  m_pTrans = NULL;
  m_pRec = NULL;
  m_server = server;
  m_port = port;
  m_pData = new char[MAXDATASIZE];
}

SyncConnClient::~SyncConnClient()
{
  if (m_pTrans != NULL)
    delete m_pTrans;
  if (m_pRec != NULL)
    delete m_pRec;
  delete [] m_pData;
}

bool SyncConnClient::SendRequest(string & result, const string & request)
{  
  m_pRec = GetReceiver(m_server.c_str(), m_port+1);

  int i;
  //char tmp[2048];
  int max = MAXDATASIZE;
  for (i=0; i<5000; i++) {
    if (m_pRec->Get(m_pData, max)) {
      cout << "Got initial message: " << m_pData << endl;    
      break;
    } else {
      //cout << "Receive error!" << endl;
      usleep(100000);
    }
  }
  delete m_pRec;
  m_pRec = NULL;

  //cout << "Sending request." << endl;
  m_pTrans = GetTransmitter(m_port+2);
  m_pTrans->SendWait(request.c_str());
  //cout << "Done." << endl;

  delete m_pTrans;
  m_pTrans = NULL;

  // Collect the result now.
  m_pRec = GetReceiver(m_server.c_str(), m_port+1);
  for (i=0; i<5000; i++) {
    if (m_pRec->Get(m_pData, max)) {
      //cout << "Got result message: " << tmp << endl;
      break;
    } else {
      //cout << "Receive error!" << endl;
      usleep(100000);
    }
  }
  delete m_pRec;
  m_pRec = NULL;

  result = m_pData;
  return true;

}

