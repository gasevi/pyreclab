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

   void close();

   bool isOpen();

   void write( std::vector<std::string>& line );

   void write( std::string& key, std::vector<std::string>& line );

private:

   enum EFileFormat
   {
      UNDEFINED,
      CSV,
      JSON
   };

   std::string m_filename;

   char m_dlm;

   char m_fieldsep;

   std::ofstream m_outfile;

   EFileFormat m_fileFormat;

};

#endif // __DATA_WRITER_H__

