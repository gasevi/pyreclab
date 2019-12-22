#include "ProgressBar.h"

#include <iostream>
#include <iomanip>
#include <iostream>

using namespace std;


ActiveProgressBar::ActiveProgressBar( float maxValue )
: m_maxValue( maxValue ),
  m_barWidth( 50 )
{
   m_startTime = time( NULL );
   m_lastUpdateTime = time( NULL );
}

ActiveProgressBar::~ActiveProgressBar()
{
   update( m_maxValue );
   cout << endl;
}

void ActiveProgressBar::update( float progress )
{
   time_t now = time( NULL );
   double secs = difftime( now, m_lastUpdateTime );
   if( secs < m_kMinUpdateTime && progress != m_maxValue )
   {
      return;
   }
   m_lastUpdateTime = now;
   float completion_rate = progress / m_maxValue;
   cout << "[";
   size_t pos = m_barWidth * completion_rate;
   for( size_t i = 0 ; i < m_barWidth ; ++i )
   {
      if (i < pos) cout << "=";
      else if (i == pos) cout << ">";
      else cout << " ";
   }
   cout << "] "
        << std::setw( 5 ) << fixed << setprecision( 1 ) << completion_rate * 100 << " % - "
        << elapsedTime( now ) << " "
        << timeOfArrival( progress, now ) << "\r";
   cout.flush();
}

string ActiveProgressBar::elapsedTime( time_t& now )
{
   double elapsedTimeSec = difftime( now, m_startTime );
   tm elapsedTime = { 0 };
   elapsedTime.tm_hour = elapsedTimeSec / 3600;
   elapsedTime.tm_min = elapsedTimeSec / 60;
   elapsedTime.tm_sec = static_cast<int>( elapsedTimeSec ) % 60;
   char buffer[32];
   strftime( buffer, 32, "ET %H:%M:%S", &elapsedTime );
   return string( buffer );
}

string ActiveProgressBar::timeOfArrival( float progress, time_t& now )
{
   float completion_rate = progress / m_maxValue;
   double elapsedTimeSec = difftime( now, m_startTime );
   double speed = completion_rate / elapsedTimeSec;
   double etaSec = ( 1 - completion_rate ) / speed;
   tm eta = { 0 };
   eta.tm_hour = etaSec / 3600;
   eta.tm_min = etaSec / 60;
   eta.tm_sec = static_cast<int>( etaSec ) % 60;
   char buffer[32];
   strftime( buffer, 32, "ETA %H:%M:%S", &eta );
   return string( buffer );
}

ProgressBar::ProgressBar( float maxValue, bool active )
: m_progressBar( NULL )
{
   set( maxValue, active );
}

ProgressBar::~ProgressBar()
{
   if( NULL != m_progressBar )
   {
      delete m_progressBar;
   }
}

void ProgressBar::set( float maxValue, bool active )
{
   if( NULL != m_progressBar )
   {
      delete m_progressBar;
   }

   if( active )
   {
      m_progressBar = new ActiveProgressBar( maxValue );
   }
   else
   {
      m_progressBar = new IdleProgressBar();
   }
}

void ProgressBar::update( float progress )
{
   m_progressBar->update( progress );
}


