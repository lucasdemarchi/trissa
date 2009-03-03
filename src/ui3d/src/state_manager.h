#ifndef _TRISSA_STATEMANAGER_H_
#define _TRISSA_STATEMANAGER_H_ 1
#include <vector>
#include <map>

typedef enum {
	STARTUP,
	GUI,
//	LOADING,
//	CANCEL_LOADING,
	GAME,
	SHUTDOWN
} GameState;

class StateManager {

public:
	StateManager(GameState state);
	virtual ~StateManager();

	GameState getCurrentState();

	bool lockState();
	bool unlockState();

	bool requestStateChange(GameState state);

//	void setFrameTime(float ms);
//	inline float getFrameTime() { return m_frame_time; }

protected:
	GameState mState;
	bool mLocked;
//	float mFrameTime;
};

#endif //_TRISSA_STATEMANAGER_H_
