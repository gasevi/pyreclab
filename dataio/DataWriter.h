#ifndef __DATA_WRITER_H__
#define __DATA_WRITER_H__

#include <vector>
#include <string>
#include <fstream>

class DataWriter
{
public:

   DataWriter();

   ~DataWriter();

   bool open( std::string& filename, char dlm = ',', std::string header = "" );

   size_t write( std::vector<std::string>& line );

   size_t write( std::string& key, std::vector<std::string>& line );

   bool isOpen();

private:

   std::string m_filename;

   char m_dlm;

   char m_fieldsep;

   std::ofstream m_outfile;

};

#endif // __DATA_WRITER_H__

