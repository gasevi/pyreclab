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

   std::string elapsedTime();

   std::string timeOfArrival( float progress );

   float m_maxValue;

   size_t m_barWidth;

   time_t m_startTime;

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

