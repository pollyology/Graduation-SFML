#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

class Button : public Text
{
public:
	void setup(const Font& font, const RenderWindow& window, const string& str, unsigned int size, const Vector2f& offsetPos);
	void update(Vector2i mousePos, bool mousePressed, bool mousePreviouslyPressed);

	// Set button settings
	void setColor(const Color& color) { m_color = color; }
	void setHoverColor(const Color& color) { m_hoverColor = color; }
	void setButtonScale(const Vector2f& scale) { m_scale = scale; }
	void setHoverScale(const Vector2f& scale) { m_hoverScale = scale; }
	void setButtonBounds(const FloatRect& bounds) { m_buttonBounds = bounds; }

	// Get button state
	bool isClicked() const { return m_isClicked; }
	bool isHovered() const { return m_isHovered; }

private:
	FloatRect m_buttonBounds;

	// Default button settings
	bool m_isHovered = false;
	bool m_isClicked = false;
	Color m_color = Color::White;
	Color m_hoverColor = Color::Yellow;
	Vector2f m_scale = { 1.0f, 1.0f };
	Vector2f m_hoverScale = { 1.075, 1.075f };
};

class SpriteButton
{
public:
	void setup(const Texture& texture, const RenderWindow& window, const Vector2f& offsetPos);
	void update(Vector2i mousePos, bool mousePressed, bool mousePreviouslyPressed);

	// Get button state
	bool isClicked() const { return m_isClicked; }
	bool isHovered() const { return m_isHovered; }
	Sprite& getSprite() { return m_sprite; }
	const Sprite& getSprite() const { return m_sprite; }

	// Set button settings
	void setColor(const Color& color) { m_color = color; }
	void setHoverColor(const Color& color) { m_hoverColor = color; }
	void setButtonScale(const Vector2f& scale) { m_scale = scale; }
	void setHoverScale(const Vector2f& scale) { m_hoverScale = scale; }
	void setButtonPosition(const Vector2f& position)
	{
		m_sprite.setPosition(position);
		m_buttonBounds = m_sprite.getGlobalBounds();
	}

	// Methods
	void rotate(float angle) { m_sprite.rotate(angle); }
	void draw(RenderWindow& window) const { window.draw(m_sprite); }

private:
	Sprite m_sprite;
	Texture m_texture;
	FloatRect m_buttonBounds;

	// Default button settings
	bool m_isHovered = false;
	bool m_isClicked = false;
	Color m_color = Color::White;
	Color m_hoverColor = Color::Yellow;
	Vector2f m_scale = { 1.0f, 1.0f };
	Vector2f m_hoverScale = { 1.1f, 1.1f };

};
