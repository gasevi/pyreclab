#include "DataReader.h"

#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

DataReader::DataReader( string filename, char dlm, bool header )
: m_filename( filename ),
  m_dlm( dlm ),
  m_header( header ),
  m_lineNumber( 0 ),
  m_infile( m_filename.c_str() )
{
}

void DataReader::readline( vector<string>& lineFields )
{
   if( m_infile.is_open() )
   {
      string s;
      while( getline( m_infile, s ) )
      {
         m_lineNumber++;

         s = strip( s );
         if( s.empty() )
         {
            continue;
         }

         if( m_header )
         {
            m_header = false;
            continue;
         }

         break;
      }

      if( !s.empty() )
      {
         istringstream ss( s );
         while( ss )
         {
            string s;
            if( !getline( ss, s, m_dlm ) )
            {
               break;
            }
            s = strip( s );
            lineFields.push_back( s );
         }
      }
   }
   else
   {
      cerr << "File [" << m_filename << "] couldn't be opened" << endl;
   }
}

bool DataReader::eof()
{
   return m_infile.eof();
}

const char* DataReader::filename()
{
   return m_filename.c_str();
}

int DataReader::currentline()
{
   return m_lineNumber;
}

string DataReader::strip( string& str )
{
   char* toremove = const_cast<char*>( " \t\n\r\f\v" );
   size_t first = str.find_first_not_of( toremove );
   size_t last = str.find_last_not_of( toremove );
   if( ( first != string::npos ) && ( last != string::npos ) )
   {
      return str.substr( first, ( last - first + 1 ) );
   }
   else
   {
      str.clear();
   }
   return str;
}


