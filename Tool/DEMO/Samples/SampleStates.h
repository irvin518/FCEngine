

#ifndef _SAMPLE_STATES_H_
#define _SAMPLE_STATES_H_


class gxSampleState
{
    friend class gxStateStack;
public:

    gxSampleState();
	virtual	~gxSampleState();

	virtual void Update(int dt)   = 0;

	virtual void	Render()        = 0;  

	virtual void	Pause();

	virtual void	Resume();
			
  
};


////////////////////////////////////////////////////////////////////////////////////////////////////



#endif	//_GAME_STATES_H_