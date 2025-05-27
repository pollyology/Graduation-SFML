#include "Engine.h"
#define M_PI 3.1415926535897932384626433

Engine::Engine()
{
	//	+---------------------------+
	//	|	WINDOW INITIALIZATION	|
	//	+---------------------------+
	setupWindow(m_Window);

	//	+---------------------------+
	//	|	MUSIC INITIALIZATION	|
	//	+---------------------------+
	if (m_music.openFromFile(FILE_MUSIC)) cout << "Success! Music file loaded \n";
	m_music.setLoop(true);
	m_music.setVolume(25);

	//	+---------------------------+
	//	|		SPRITE SETTINGS		|
	//	+---------------------------+
	m_characterMap[TWILIGHT_SPARKLE] =
	{
		"Twilight Sparkle",
		TWILIGHT_SPARKLE,
		Color(154, 218, 248, 155),						// Background color (Rainbow Dash Blue lol)
		Vector2f(0.190, 0.190),
		Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
		Vector2f(-85, 115),
		10,
		0.025f
	};

	if (m_texture.loadFromFile(TWILIGHT_SPARKLE)) cout << "Success! Twilight texture loaded \n";
	m_sprite.setTexture(m_texture);

	// Save settings reference
	auto settings = m_characterMap[TWILIGHT_SPARKLE];

	// Sprite Settings
	m_sprite.setScale(settings.scale); // Tweak to adjust sprite size

	// Center origin and position
	FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2, bounds.height / 2);
	m_sprite.setPosition(settings.position + settings.offset);

	//	+-------------------------------+
	//	|  UI/ANIMATION INITIALIZATION	|
	//	+-------------------------------+
	initUI();
	initAnim();

}

void Engine::run()
{
	Clock clock;
	cout << "Con-graduate-lations!" << endl;

	while (m_Window.isOpen())
	{
		Time time = clock.restart();
		float dt = time.asSeconds();

		input();
		update(dt);
		draw();
	}
}

void Engine::input()
{
	Event event;
	static bool mouseClickPrevious = false;	// Tracks if mouse was clicked last frame
	bool const mouseLeftPressed = Mouse::isButtonPressed(Mouse::Left);

	Vector2i relativePos(Mouse::getPosition(m_Window));										 // Gets mouse position relative to window size
	Vector2i const mousePos = static_cast<Vector2i>(m_Window.mapPixelToCoords(relativePos)); // Adjusts mouse position to work with window resize

	// === Update Button States ===
	m_specialButton.update(mousePos, mouseLeftPressed, mouseClickPrevious);

	if (!m_sunIsClicked && !m_sunAnimating && !m_sunIsSpinning)		// This condition stops button logic after clicking
	{
		m_sunSprite.update(mousePos, mouseLeftPressed, mouseClickPrevious);
	}

	// === Experiment with range to change shape diversity ===
	int min = 8;	// If min < 8, program will sometimes create triangle particles
	int max = 20;
	int numParticles = 5 + rand() % 6; // number of particles you want to create per click

	while (m_Window.pollEvent(event))
	{
		if (event.type == Event::Closed || event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
		{
			m_Window.close();
		}
		//	+---------------------------+
		//	|		TITLE SCREEN		|	When sun is clicked, begins an animation.
		//	+---------------------------+
		
		if (m_sunSprite.isClicked() && !m_sunIsClicked)
		{
			m_sunIsClicked = true;
			m_sunAnimating = true;
			m_sunAnimTimer = 0.0f;

			m_sunStartPos = m_sunSprite.getSprite().getPosition();
			m_sunEndPos = Vector2f(WINDOW_WIDTH - 135.f, 135.f); // Adjust as needed (sets sun end position to top right corner)
			m_sunStartScale = m_sunSprite.getSprite().getScale();
			m_sunEndScale = Vector2f(0.50f, 0.50f); // Shrink size

			m_music.play();
			m_music.setPlayingOffset(seconds(0.40f));	// Pretty hardcoded solution, I timed the music to line up with sun animation
		}

		//	+---------------------------+
		//	|		SPECIAL BUTTON		|	When clicked, spawns random number of stars to fall down screen.
		//	+---------------------------+
		m_specialButtonClicked = false;

		if (m_specialButton.isClicked() && !m_specialButtonClicked && !m_twilightIsScrolling)
		{
			m_specialButtonClicked = true;

			int random = rand() % (2 - 1 + 1) + 1;
			for (int i = 0; i < random; i++)
			{
				specialEvent();
			}
		}

		//	+---------------------------+
		//	|		PARTICLES LOGIC		|	When single-clicking mouse or holding click-drag, spawns particles on-screen
		//	+---------------------------+
		if (mouseLeftPressed && !m_twilightIsScrolling)
		{
			if (mouseClickPrevious)	// If holding click, limit particles made per frame
			{
				numParticles = rand() % 3 + 1;
			}

			for (int i = 0; i < numParticles; i++)
			{
				int random = rand() % (max - min + 1) + min;
				if (random % 2 == 0) { random++; } // Ensures random number is odd, even numbers create 'floppy' shapes
				int numPoints = random;

				Particle particle(m_Window, numPoints, mousePos);
				particle.setTTL(2.75);
				m_particles.emplace_back(particle);
			}
		}

		//	+---------------------------+
		//	|		RESTART BUTTON		|	Hidden way to restart animation, disabled.
		//	+---------------------------+
		float cooldownTimer = m_restartCooldown.getElapsedTime().asSeconds();
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::R && m_sunIsClicked && cooldownTimer > 0.5f)
		{
			//reset();
			//m_restartCooldown.restart();

		}

		mouseClickPrevious = mouseLeftPressed;	// Always keep this at the end of the input loop
	}
}

void Engine::update(float dtAsSeconds)
{
	//	+-------------------+	Sun will spin and shrink in size as it travels to the top right corner.
	//	|	SUN ANIMATION	|	After travelling, it will stay in place and continue to spin while pulsing.
	//	+-------------------+
	if (m_sunIsSpinning) m_sunSprite.rotate(90.f * dtAsSeconds);

	if (m_sunAnimating)
	{
		m_sunAnimTimer += dtAsSeconds;

		float t = m_sunAnimTimer / m_sunAnimDuration; // Ratio of elapsed time and animation length
		if (t >= 1.0f)
		{
			t = 1.0f;
			m_sunAnimating = false;
			m_sunIsSpinning = true;
			m_sunBaseScale = m_sunEndScale; // Set the final scale as the pulse baseline
		}

		// Set sun's position, scale, and rotation to change based on time left in animation (dt)
		Vector2f newPos = m_sunStartPos + t * (m_sunEndPos - m_sunStartPos);
		Vector2f newScale = m_sunStartScale + t * (m_sunEndScale - m_sunStartScale);
		float rotationAngle = 360.f * t;	// Rotate in full circle, over time

		// Animate sun's position, scale, and rotation
		m_sunSprite.getSprite().setPosition(newPos);
		m_sunSprite.getSprite().setScale(newScale);
		m_sunSprite.getSprite().setRotation(rotationAngle);

		if (!m_sunAnimating) // sun animation just finished
		{
			m_sunIsSpinning = true;
			m_sunBaseScale = m_sunEndScale; // prevent sun from suddenly shrinking

			m_particles.clear();

			// Start Twilight Sparkle sprite scrolling up
			m_twilightIsScrolling = true;
			m_twilightScrollTimer = 0.0f;

			// Reset Twilight Sparkle sprite to start position
			m_sprite.setPosition(m_twilightStartPos);
		}
	}
	//	+--------------------+	
	//	| TWILIGHT ANIMATION |	After sun finishes its animation, Twilight Sparkle will scroll up from offscreen.
	//	+--------------------+
	if (m_twilightIsScrolling)
	{
		m_twilightScrollTimer += dtAsSeconds;
		float t = m_twilightScrollTimer / m_twilightScrollDuration;
		if (t >= 1.0f)
		{
			t = 1.0f;
			m_twilightIsScrolling = false;
		}
		// Over time, will linearly set sprite from its start to end position
		Vector2f newPos = m_twilightStartPos + t * (m_twilightEndPos - m_twilightStartPos);
		m_sprite.setPosition(newPos);
	}
	//	+---------------------+	
	//	| SUN PULSE/HEARTBEAT |	Sun pulses by alternating its scale between an 'inner' and 'outer' pulse.
	//	+---------------------+
	else if (m_sunIsSpinning && !m_twilightIsScrolling) // Sun starts pulsing before Twilight begins scrolling up
	{
		// Constant rotation (rotate by 90 degrees per second)
		m_sunSprite.rotate(90.f * dtAsSeconds);

		// Heartbeat pulse logic
		m_sunBeatTimer += dtAsSeconds;

		if (m_sunBeatTimer >= m_sunBeatInterval)
		{
			m_sunBeatTimer = 0.0f;
			m_sunBeatGrowing = !m_sunBeatGrowing;
		}

		float scaleOffset = (m_sunBeatGrowing)
			? m_sunBeatTimer / m_sunBeatInterval
			: 1.0f - (m_sunBeatTimer / m_sunBeatInterval);

		float pulseScale = 1.0f + m_sunBeatScaleAmount * scaleOffset; // Scale of each pulse

		// Apply pulse to the base scale
		Vector2f beatScale = m_sunBaseScale * pulseScale;
		m_sunSprite.getSprite().setScale(beatScale);

		// Apply subtle pulse to base scale, improves visual
		beatScale = m_sunBaseScale * pulseScale;
		m_sunSprite.getSprite().setScale(beatScale);
		
		//	+--------------------+	
		//	| UPDATING PARTICLES |	Start updating particle TTL after sun and scroll animation finished.
		//	+--------------------+
		FloatRect killbox(0, WINDOW_HEIGHT, WINDOW_WIDTH, 100); // Despawn particles that fall off-screen

		// Logic for updating particles and TTL
		for (auto it = m_particles.begin(); it != m_particles.end();)
		{
			if (it->getTTL() > 0.0)
			{
				it->update(dtAsSeconds);
				it++;
			}
			else if (it->getPosition().y > killbox.top)
			{
				it = m_particles.erase(it);
			}
			else
			{
				it = m_particles.erase(it);
			}
		}
	}
	//	+-----------------+	
	//	| FLOATING CLOUDS |	Clouds float from left to right of screen, then once off-screen, resets position to loop again
	//	+-----------------+
	for (size_t i = 0; i < m_cloudSprites.size(); ++i)
	{
		Sprite& cloud = m_cloudSprites[i];
		float speed = m_cloudSpeeds[i];

		// Move cloud rightwards over time
		cloud.move(speed * dtAsSeconds, 0.0f);

		// If cloud fully offscreen right, reset to left
		if (cloud.getPosition().x - cloud.getLocalBounds().width * 0.25f > WINDOW_WIDTH)
		{
			cloud.setPosition(-cloud.getLocalBounds().width * 0.25f, cloud.getPosition().y);
		}
	}

	//	+---------------------------+	
	//	| SPECIAL BUTTON PULSE/TILT |	'Congratulations!' button will begin pulsing and tilting left and right.
	//	+---------------------------+
	// Animate special button pulse and tilt
	m_specialPulseTimer += dtAsSeconds;
	m_specialTiltTimer += dtAsSeconds;

	// Pulse calculation (scale oscillates between min and max)
	float pulseProgress = fmod(m_specialPulseTimer, m_specialPulseDuration) / m_specialPulseDuration;  // 0->1 loop

	// Use a sine wave to smoothly scale between min and max, mathematical!
	float pulseScale = m_specialPulseMinScale + (m_specialPulseMaxScale - m_specialPulseMinScale) * (0.5f + 0.5f * sin(2 * M_PI * pulseProgress));

	// Tilt calculation (rotation oscillates between -maxAngle and +maxAngle)
	float tiltProgress = fmod(m_specialTiltTimer, m_specialTiltDuration) / m_specialTiltDuration; // 0->1 loop

	// Sine wave for rotation angle, oscillates between -maxAngle and +maxAngle
	float tiltAngle = m_specialTiltMaxAngle * sin(2 * M_PI * tiltProgress);

	// Apply scale and rotation to the special button's sprite or drawable
	m_specialButton.setScale(Vector2f(pulseScale, pulseScale));
	m_specialButton.setRotation(tiltAngle);
}

void Engine::draw()
{
	// Drawing
	const auto& settings = m_characterMap[TWILIGHT_SPARKLE];
	Color color = settings.backgroundColor;

	m_Window.clear(color);

	// Draw clouds
	for (auto& cloud : m_cloudSprites)
	{
		m_Window.draw(cloud);
	}

	// Draw Twilight Sparkle sprite OVER clouds
	if (m_sunIsClicked)
	{
		m_Window.draw(m_sprite);		// Twilight Sparkle
		m_Window.draw(m_specialButton); // 'Congratulations!'
	}

	// Draw sun before and after game start
	if (!m_sunIsClicked)
	{
		m_Window.draw(m_gameTitle);		// 'Click me!'
		m_sunSprite.draw(m_Window);		// Sun
	}
	else m_sunSprite.draw(m_Window);

	// Draw particles
	if (!m_sunAnimating && m_sunIsClicked) // Only draw particles after sun animation is done
	{
		for (auto& particle : m_particles)
		{
			m_Window.draw(particle);
		}
	}
	m_Window.display();
}

void Engine::specialEvent()
{
	m_spawnBox = FloatRect(0, -50, WINDOW_WIDTH, 5);	// left, top, width, height
	int min = 8;
	int max = 13;
	int numParticles = rand() % 10 + 7;

	int random = rand() % (max - min + 1) + min;
	if (random % 2 == 0) { random++; } // Ensures random number is odd, even numbers create 'floppy' shapes
	int numPoints = random;

	for (int i = 0; i < numParticles; i++)
	{
		float x = m_spawnBox.left + static_cast<float>(rand()) / RAND_MAX * m_spawnBox.width;
		float y = m_spawnBox.top + static_cast<float>(rand()) / RAND_MAX * m_spawnBox.height;
		Vector2i randomPos(static_cast<int>(x), static_cast<int>(y));

		Particle particle(m_Window, numPoints, randomPos);

		// Setting physics for special particles
		const float vx = 1.0f;
		const float vy = 5.0f;
		float ttl = 3.0f + static_cast<float>(rand()) / RAND_MAX * 3.0f; // Between 3 and 6

		particle.setVelocityX(vx);
		particle.setVelocityY(vy);
		particle.setTTL(ttl);

		m_particles.emplace_back(particle);
	}

	//cout << "Exited special event. \n";
}

void Engine::initUI()
{
	//	+-----------------------+
	//	|	TEXT AND BUTTONS	|
	//	+-----------------------+

	// Load font
	m_font.loadFromFile(FONT_FILE);

	// Game Buttons
	m_gameTitle.Button::setup(m_font, m_Window, "click me!", 75, Vector2f(0, -300));				// Game Title
	m_gameTitle.setOutlineColor(Color::Black);
	m_gameTitle.setOutlineThickness(5);

	m_specialButton.Button::setup(m_font, m_Window, "congratulations!", 70, Vector2f(0, -175));		// Special Button
	m_specialButton.setOutlineColor(Color::Black);
	m_specialButton.setOutlineThickness(5);
}

void Engine::initAnim()
{
	//	+---------------------------+
	//	|	SPRITES AND ANIMATIONS	|
	//	+---------------------------+

	// Twilight Sparkle
	FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2, bounds.height / 2);

	m_twilightEndPos = m_characterMap[TWILIGHT_SPARKLE].position + m_characterMap[TWILIGHT_SPARKLE].offset; // Final position of Sprite
	m_twilightStartPos = Vector2f(m_twilightEndPos.x, WINDOW_HEIGHT + bounds.height / 2);					// Start Sprite offscreen below window

	m_sprite.setPosition(m_twilightStartPos);

	// Load and setup sun texture
	if (m_sunTexture.loadFromFile(IMAGE_SUN)) cout << "Success! Sun asset loaded \n";
	m_sunSprite.setup(m_sunTexture, m_Window, Vector2f(0, 25));

	// Load cloud texture
	if (m_cloudTexture.loadFromFile(IMAGE_CLOUD)) cout << "Success! Cloud asset loaded \n";

		// Initialize multiple clouds with random positions & speeds
		for (int i = 0; i < 5; ++i)
		{
			Sprite cloud(m_cloudTexture);

			// Set origin, scale if needed
			cloud.setOrigin(cloud.getLocalBounds().width / 2, cloud.getLocalBounds().height / 2);
			cloud.setScale(0.25f, 0.25f);

			// Random horizontal start positions either offscreen left or somewhere on screen
			float startX = -(rand() % 500 + 50); // between -50 and -550 px

			// Random vertical position in upper half of the window
			float startY = static_cast<float>(rand() % (WINDOW_HEIGHT / 2));

			cloud.setPosition(startX, startY);

			m_cloudSprites.push_back(cloud);

			// Random speed between 20 to 80 pixels per second
			m_cloudSpeeds.push_back(static_cast<float>((rand() % 60) + 20));
		}

	cout << "Wassup.\n";
}

void Engine::reset()
{
	m_sunIsClicked = false;
	m_sunAnimating = false;
	m_sunIsSpinning = false;
	m_specialButtonClicked = false;
	m_twilightIsScrolling = false;

	m_sunAnimTimer = 0.0f;
	m_sunBeatTimer = 0.0f;
	m_twilightScrollTimer = 0.0f;

	m_particles.clear();

	m_sunSprite.getSprite().setPosition(m_sunStartPos);
	m_sunSprite.getSprite().setScale(m_sunStartScale);
	m_sunSprite.getSprite().setRotation(0.0);

	m_sprite.setPosition(m_twilightStartPos);

	m_music.stop();
	m_music.setPlayingOffset(seconds(0.0));
}