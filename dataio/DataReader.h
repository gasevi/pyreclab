#ifndef __DATAREADER_H__
#define __DATAREADER_H__

#include <vector>
#include <string>
#include <fstream>

class DataReader
{
public:

   DataReader( std::string filename, char dlm = ',', bool header = false );

   std::vector<std::string> readline();

   bool eof();

private:

   static
   std::string strip( std::string& str );

   std::string m_filename;

   char m_dlm;

   bool m_header;

   std::ifstream m_infile;

};

#endif // __DATAREADER_H__

