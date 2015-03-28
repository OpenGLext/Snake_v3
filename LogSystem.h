#pragma once

#include <time.h>
#include <string>
#include <fstream>

namespace LogSpace
{
//---------------------------------------------------------------------------
#define OUT_FILE ("log.htm")
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

class CLog
{
public:       

  CLog();
  ~CLog();

  void setSize(std::string const& s);

  void setColor(std::string const& c);

  void setFace(std::string const& f);

  void add(std::string const& msg);

  void bold(std::string const&/*unused*/){ changedFlags_[3] = true; };

  CLog& operator<<(size_t val);

  CLog& operator<<(int val);

  CLog& operator<<(double val);

private:

  std::ofstream file_;
  bool changedFlags_[4];
  std::string size_, color_, face_;
};
//---------------------------------------------------------------------------
class CHolder
{
  typedef void(CLog::*F)(std::string const&);

public:

  CHolder(F f, std::string const& msg)
    :f_(f), msg_(msg)
  {
  }

  CLog& operator()(CLog& l)
  {
    (l.*f_)(msg_);
    return l;
  }

private:

  F f_;
  std::string const& msg_;
};
//---------------------------------------------------------------------------
inline CLog::CLog()
{
  file_.open(OUT_FILE,std::ios::app);
  file_.close();

  memset(changedFlags_,0,sizeof(changedFlags_));  

  add("<title>Log</title>");
  add("<html><body BgColor=cornsilk>");

  setSize("3");
  setColor("darkblue");
  setFace("verdana");

  add("<b> ...:[Snake v3.0 Log]:... </b><br><br>");

  setSize("2");
}
//---------------------------------------------------------------------------
inline CLog::~CLog()
{
  add("</font></body></html>");
}
//---------------------------------------------------------------------------
inline void CLog::setSize(std::string const& s)
{
  if(size_ != s) 
  {  
    size_ = s;
    changedFlags_[0] = true;
  }
}
//---------------------------------------------------------------------------
inline void CLog::setColor(std::string const& c)
{
  if(color_ != c) 
  {
    color_ = c;
    changedFlags_[1] = true;
  }
}
//---------------------------------------------------------------------------
inline void CLog::setFace(std::string const& f)
{
  if(face_ != f)
  {
    face_ = f;
    changedFlags_[2] = true;
  }
}

//---------------------------------------------------------------------------
inline void CLog::add(std::string const& msg)
{  
  file_.open(OUT_FILE, std::ios::app);

  if(changedFlags_[0] || changedFlags_[1] || changedFlags_[2])
  {
    file_ << "</font>" << '\n';
    file_ << "<font size=" << size_.c_str() <<
         " color="     << color_.c_str() <<  
         " face="     << face_.c_str() << ">";

    changedFlags_[0] = changedFlags_[1] = changedFlags_[2] = false;
  }
  //if bold text
  if(changedFlags_[3])
  {
    file_ << "<b>" << msg.c_str() << "</b>";
    changedFlags_[3] = false;
  }
  else
    file_ << msg.c_str();
   
    file_.close();       
}
//---------------------------------------------------------------------------
inline CLog& CLog::operator<<(size_t val)
{
  file_.open(OUT_FILE, std::ios::app);
  file_ << static_cast<unsigned int>(val);//disable warning C4267 :)
  file_.close();

  return *this;
}
//---------------------------------------------------------------------------
inline CLog& CLog::operator<<(int val)
{
  file_.open(OUT_FILE, std::ios::app);
  file_ << val;
  file_.close();

  return *this;
}
//---------------------------------------------------------------------------
inline CLog& CLog::operator<<(double val)
{
  file_.open(OUT_FILE, std::ios::app);
  file_ << val;
  file_.close();

  return *this;
}
//---------------------------------------------------------------------------
inline CLog& operator<<(CLog& l, CHolder& h)
{  
  return h(l);
}
//---------------------------------------------------------------------------
inline CLog& operator<<(CLog& l, std::string const& msg)
{  
  l.add(msg);
  return l;
}
//---------------------------------------------------------------------------
inline CLog& operator<<(CLog& l, unsigned char const* msg)
{  
  char const* cmsg = reinterpret_cast<char const*>(msg);
  std::string str(cmsg, strlen(cmsg));
  l << str;
  return l;
}
//---------------------------------------------------------------------------
inline CLog& operator<<(CLog& l, CHolder (*f)())
{  
  return f()(l);
}
//---------------------------------------------------------------------------
inline CHolder color(std::string const& val)
{
  return CHolder(&CLog::setColor, val);
}
//---------------------------------------------------------------------------
inline CHolder font(std::string const& val)
{
  return CHolder(&CLog::setFace, val);
}
//---------------------------------------------------------------------------
inline CHolder size(std::string const& val)
{
  return CHolder(&CLog::setSize, val);
}
//---------------------------------------------------------------------------
inline CHolder endl()
{
  static const std::string s = "<br>";
  return CHolder(&CLog::add, s);
}
//---------------------------------------------------------------------------
inline CHolder bold()
{
  return CHolder(&CLog::bold, "");
}
//---------------------------------------------------------------------------
inline CHolder time()
{
  static std::string s(64,'\0');

  time_t t;
  t = ::time( NULL );
  ctime_s(&s[0], s.size(), &t);

  return CHolder(&CLog::add, s.append(" "));
}
//---------------------------------------------------------------------------
#undef OUT_FILE
//---------------------------------------------------------------------------
}//End of lf namespace
//---------------------------------------------------------------------------
//inline LogSpace::CLog& logFile()
//{
//	return LogSpace::CLog;
//}
//---------------------------------------------------------------------------

/*

Использование 

 logFile<< LogSpace::color("gray") << LogSpace::color("green") << "Snake length" << snake.GetLenBody()<< LogSpace::endl;


*/