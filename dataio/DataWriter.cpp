#include "DataWriter.h"
#include <iostream>

using namespace std;


DataWriter::DataWriter()
: m_dlm( ',' ),
  m_fieldsep( ' ' ),
  m_fileFormat( UNDEFINED )
{
}

DataWriter::~DataWriter()
{
   if( isOpen() )
   {
      close();
   }
}

bool DataWriter::open( string& filename, char dlm, string header )
{
   m_dlm = dlm;
   m_filename = filename;
   m_outfile.open( filename.c_str() );
   if( !header.empty() && m_outfile.is_open() )
   {
      m_outfile << header << "\n";
   }
   return m_outfile.is_open();
}

void DataWriter::close()
{
   if( m_outfile.is_open() && m_fileFormat == JSON )
   {
      m_outfile << "}" << endl;
   }
   m_outfile.close();
}

bool DataWriter::isOpen()
{
   return m_outfile.is_open();
}

void DataWriter::write( vector<string>& line )
{
   if( m_fileFormat == UNDEFINED )
   {
      m_fileFormat = CSV;
   }
   else if( m_fileFormat != CSV )
   {
      cout << "Invalid line strucutre for CSV file format. Line discarded!." << endl;
      return;
   }

   if( m_outfile.is_open() )
   {
      vector<string>::iterator ind;
      vector<string>::iterator end = line.end();
      for( ind = line.begin() ; ind != end ; ++ind )
      {
         m_outfile << *ind;
         if( ind + 1 != end )
         {
            m_outfile << m_dlm;
         }
      }
      m_outfile << "\n";
   }
}

void DataWriter::write( string& key, vector<string>& line )
{
   if( m_fileFormat == UNDEFINED )
   {
      m_fileFormat = JSON;
   }
   else if( m_fileFormat != JSON )
   {
      cout << "Invalid line structure for JSON file format. Line discarded!." << endl;
      return;
   }

   if( m_fieldsep == ' ' )
   {
      m_outfile << "{" << endl;
   }

   m_outfile << m_fieldsep << "\"" << key << "\": [";

   vector<string>::iterator end = line.end();
   vector<string>::iterator ind;
   for( ind = line.begin() ; ind != end ; ++ind )
   {  
      m_outfile << *ind; 
      if( ind + 1 != end )
      {  
         m_outfile << m_dlm;
         continue;
      }
      m_outfile << "]" << endl;
   }

   if( m_fieldsep == ' ' )
   {
      m_fieldsep = ',';
   }
}


