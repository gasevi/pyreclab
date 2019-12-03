#ifndef __PROGRESS_BAR_H__
#define __PROGRESS_BAR_H__

#include <unistd.h>

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

   ActiveProgressBar( float maxValue )
   : m_maxValue( maxValue ),
     m_barWidth( 70 )
   {
   }

   ~ActiveProgressBar();

   void update( float progress );

private:

   float m_maxValue;

   size_t m_barWidth;

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

