#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/stat.h>

#include "DataReader.h"
#include "RatingMatrix.h"
#include "DataFrame.h"
#include "AlgItemAvg.h"

using namespace std;


int main( int argc, char** argv )
{
   string trainingFile;
   string testingFile;
   int c;
   while( ( c = getopt( argc, argv, ":r:e:" ) ) != EOF )
   {
      switch( c )
      {
      case 'r':
      {
         struct stat s;
         if( ( stat( optarg, &s ) == 0 ) && ( s.st_mode & S_IFREG ) ) // is file ?
         {
            trainingFile = optarg;
         }
         else
         {
            cerr << "training dataset must be a file" << endl;
            return -1;
         }
         break;
      }

      case 'e':
      {
         struct stat s;
         if( ( stat( optarg, &s ) == 0 ) && ( s.st_mode & S_IFREG ) ) // is file ?
         {
            testingFile = optarg;
         }
         else
         {
            cerr << "testing dataset must be a file" << endl;
            return -1;
         }
         break;
      }

      case '?':
         cout << "Usage: " << argv[0] << " -r <training_file> -e <testing_file>" << endl;
         return 0;

      default:
         cerr << argv[0] << ": missing operand" << endl;
         cerr << "Usage: " << argv[0] << " -r <training_file> -e <testing_file>" << endl;
         return -1;
      }
   }

   char dlm = '\t';
   if( trainingFile.substr( trainingFile.find_last_of( "." ) + 1 ) == "csv" )
   {
      dlm = ',';
   }

   int userpos = 0, itempos = 1, ratingpos = 3;
   DataReader trainingReader( trainingFile, dlm, true );
   RatingMatrix ratingMatrix( trainingReader, userpos, itempos, ratingpos );
   AlgItemAvg itemAvg( ratingMatrix );

   cout << "before prediction" << endl;
   double predict = itemAvg.predict( "1469", "11757" );
   cout << "prediction: " << predict << endl;

/*
   userpos = 2, itempos = 0;
   DataReader testReader( testingFile, dlm, true );
   DataFrame testingData( testReader, userpos, itempos );
   mostPopular.test( testingData );
*/
   return 0;
}


