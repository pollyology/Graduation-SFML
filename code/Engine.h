#pragma once
#include "Particle.h"
#include "config.h"
#include "Button.h"
using namespace sf;
using namespace std;

struct CharacterSettings
{
	string name;			// Character name
	string filename;		// Folder name
	Color backgroundColor;	// Character color
	Vector2f scale;			// Custom scale
	Vector2f position;		// Custom position
	Vector2f offset;		// Offset from origin
	int frameCount;			// Number of frames
	float frameTime;		// Seconds per frame
};

class Engine
{
private:
	// A regular RenderWindow
	RenderWindow m_Window;

	// vector for Particles
	vector<Particle> m_particles;

	// Vector to hold textures for animation
	vector<Texture> m_frames;

	// Texture and Sprite to display character
	Texture m_texture;
	Sprite m_sprite;

		// Structures to access character
		map<string, CharacterSettings> m_characterMap;
			
	// Texture and Sprite to display clouds
	Texture m_cloudTexture;
	vector<Sprite> m_cloudSprites;
	vector<float> m_cloudSpeeds; // Holds different speeds for parallax effect

	// Music to play during program, can be toggled ON/OFF
	Music m_music;
	
	// Text to create Title Screen and Special Event buttons
	Button m_gameTitle;
	Button m_specialButton;
	Font m_font;

	//	+-------------------+
	//	|	SUN ANIMATION	|
	//	+-------------------+
	SpriteButton m_sunSprite;
	Texture m_sunTexture;

	// Sun rotation animation
	bool m_sunAnimating = false;
	float m_sunAnimTimer = 0.0f;
	Vector2f m_sunStartPos;
	Vector2f m_sunEndPos;
	Vector2f m_sunStartScale;
	Vector2f m_sunEndScale;
	float m_sunAnimDuration = 2.0f; // 2 seconds
	
	// Sun pulse animation
	float m_sunBeatTimer = 0.0f;
	float m_sunBeatInterval = 0.5f;					// Duration of seconds between each beat
	bool m_sunBeatGrowing = true;
	Vector2f m_sunBaseScale = { 0.25f, 0.25f };		// Final resting scale
	float m_sunBeatScaleAmount = 0.05f;				// How much it pulses
		
	//	+-----------------------+
	//	|	BUTTON ANIMATION	|
	//	+-----------------------+

	// Special button pulse
	float m_specialPulseTimer = 0.0f;
	float m_specialPulseDuration = 1.5f;	// Duration of a full pulse cycle (seconds)
	float m_specialPulseMinScale = 0.9f;	// Controls how small button is at start of pulse
	float m_specialPulseMaxScale = 1.1f;	// Controls how big button is at end of pulse

	// Special button tilt
	float m_specialTiltTimer = 0.0f;
	float m_specialTiltDuration = 2.0f;   // Duration of a full tilt cycle (seconds)
	float m_specialTiltMaxAngle = 10.f;  // How many degrees the buttons tilts at

	//	+-----------------------+
	//	|	TWILIGHT ANIMATION	|
	//	+-----------------------+
	Vector2f m_twilightStartPos;
	Vector2f m_twilightEndPos;

	bool m_twilightIsScrolling = false;
	float m_twilightScrollTimer = 0.0f;
	const float m_twilightScrollDuration = 1.0f;  // How long it takes for sprite to scroll up (seconds)

		// Booleans for handling button input
		bool m_specialButtonClicked;
		bool m_sunIsClicked = false;
		bool m_sunIsSpinning = false;

		// Special Event
		FloatRect m_spawnBox;

	// For restarting program
	Clock m_restartCooldown;

	// Private functions for internal use only
	void input();
	void update(float dtAsSeconds);
	void draw();

		// New functions added to initalize SFML objects
		void initUI();
		void initAnim();
		void specialEvent();
		void reset();
	
public:
	// The Engine constructor
	Engine();

	// Run will call all the private functions
	void run();

};
