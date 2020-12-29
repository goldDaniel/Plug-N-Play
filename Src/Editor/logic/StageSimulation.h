#ifndef STAGE_SIMULATION_H
#define STAGE_SIMULATION_H

class StageSimulation
{
private:

	bool running = false;


	float stage_timer = 0.f;
	float stage_length = 30.f;

public:


	void Start()
	{
		running = true;
	}

	void Pause()
	{
		running = false;
	}

	void Stop()
	{
		running = false;
	}

	bool IsRunning() const
	{
		return running;
	}

	float GetStageLength() const 
	{
		return stage_length;
	}

	float GetStageTime() const
	{
		return stage_timer;
	}

	void SetStageTime(const float time)
	{
		assert(time >= 0);

		this->stage_timer = time;
	}

};


#endif