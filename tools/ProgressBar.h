#ifndef __PROGRESS_BAR_H__
#define __PROGRESS_BAR_H__

#include <unistd.h>
#include <string>

class ProgressBarStrategy
{
public:

   virtual
   ~ProgressBarStrategy()
   {
   }

   virtual
   void update( float progress ) = 0;

};

class IdleProgressBar
      : public ProgressBarStrategy
{
public:

   void update( float progress )
   {
   }

};

class ActiveProgressBar
      : public ProgressBarStrategy
{
public:

   ActiveProgressBar( float maxValue );

   ~ActiveProgressBar();

   void update( float progress );

private:

   static
   const size_t m_kMinUpdateTime = 1;

   std::string elapsedTime( time_t& now );

   std::string timeOfArrival( float progress, time_t& now );

   float m_maxValue;

   size_t m_barWidth;

   time_t m_startTime;

   time_t m_lastUpdateTime;

};

class ProgressBar
{
public:

   ProgressBar( float maxValue, bool is_active );

   ~ProgressBar();

   void set( float maxValue, bool is_active );

   void update( float progress );

private:

   ProgressBar()
   {
   }

   ProgressBarStrategy* m_progressBar;

};

#endif // __PROGRESS_BAR_H__

