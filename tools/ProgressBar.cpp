#include "ProgressBar.h"

#include <iostream>
#include <iomanip>

using namespace std;


ActiveProgressBar::~ActiveProgressBar()
{
   update( m_maxValue );
   cout << endl;
}

void ActiveProgressBar::update( float progress )
{
   float completion_rate = progress / m_maxValue;
   cout << "[";
   size_t pos = m_barWidth * completion_rate;
   for( size_t i = 0 ; i < m_barWidth ; ++i )
   {
      if (i < pos) cout << "=";
      else if (i == pos) cout << ">";
      else cout << " ";
   }
   cout << "] " << fixed << setprecision( 1 ) << completion_rate * 100.0 << " %\r";
   cout.flush();
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


