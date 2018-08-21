#ifndef __DATAREADER_H__
#define __DATAREADER_H__

#include <vector>
#include <string>
#include <fstream>

class DataReader
{
public:

   DataReader( std::string filename, char dlm = ',', bool header = false );

   void readline( std::vector<std::string>& lineFields );

   bool eof();

   const char* filename();

   int currentline();

private:

   static
   std::string strip( std::string& str );

   std::string m_filename;

   char m_dlm;

   bool m_header;

   int m_lineNumber;

   std::ifstream m_infile;

};

#endif // __DATAREADER_H__

