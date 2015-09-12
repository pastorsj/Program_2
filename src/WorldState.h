#ifndef __WORLDSTATE_H
#define __WORLDSTATE_H

#define NUM_TRACKED_FRAMES 10
#define MOVEMENT_PER_SECOND 1.0

class WorldState
{
private:
	float frameTimes[NUM_TRACKED_FRAMES];
	float currentTime;
	bool running;
	float xOffset, yOffset;

public:
	WorldState()
	{
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = 0.0f;
		running = true;
		xOffset = 0.0f;
		yOffset = 0.0f;
	}
	
	void updateFrameTime(float timeAsSeconds)
	{
		for(size_t i=1; i<NUM_TRACKED_FRAMES; i++)
			frameTimes[i] = frameTimes[i-1];
		frameTimes[0] = timeAsSeconds;
	}
	
	void printFPS() const
	{
		float sum = 0.0f;
		float avg = 0.0f;
		float fps = 0.0f;
		
		for(size_t i=0; i<NUM_TRACKED_FRAMES; i++)
			sum += frameTimes[i];
		
		avg = sum / NUM_TRACKED_FRAMES;
		fps = 1.0f / avg;
		printf("fps %f\n", fps);
	}
	
	void setRunning(bool r)
	{
		running = r;
	}

	bool isRunning() const
	{
		return running;
	}

	float getCurrentTime() const
	{ return this->currentTime; }

	float getXOffset() const
	{ return this->xOffset; }

	float getYOffset() const
	{ return this->yOffset; }

	void timeStep(float t)
	{
		this->updateFrameTime( t - this->currentTime );
		this->currentTime = t;
	}

	void moveRight()
	{
		this->xOffset = this->xOffset + this->frameTimes[0] * MOVEMENT_PER_SECOND;
	}

	void moveLeft()
	{
		this->xOffset = this->xOffset - this->frameTimes[0] * MOVEMENT_PER_SECOND;
	}

	void moveUp()
	{
		this->yOffset = this->yOffset + this->frameTimes[0] * MOVEMENT_PER_SECOND;
	}

	void moveDown()
	{
		this->yOffset = this->yOffset - this->frameTimes[0] * MOVEMENT_PER_SECOND;
	}


};

#endif
