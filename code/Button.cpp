#include "Button.h"

void Button::setup(const Font& font, const RenderWindow& window, const string& str, unsigned int size, const Vector2f& offsetPos)
{
	// Default text settings
	setFont(font);
	setFillColor(Color::White);
	setString(str);
	setCharacterSize(size);
	setScale(m_scale);
	setHoverScale(m_hoverScale);

	// Center origin of button
	FloatRect bounds = getLocalBounds();
	setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);

	// Set position with the offset
	setPosition(window.getSize().x / 2 + offsetPos.x, window.getSize().y / 2 + offsetPos.y);

	// Set button bounds
	m_buttonBounds = getGlobalBounds();
}

void Button::update(Vector2i mousePos, bool mousePressed, bool mousePreviouslyPressed)
{
	bool hovered = m_buttonBounds.contains(static_cast<Vector2f>(mousePos));
	m_isClicked = false;

	if (hovered)
	{
		if (!m_isHovered)
		{
			m_isHovered = true;
			setFillColor(m_hoverColor);
			setScale(m_hoverScale);
		}

		if (mousePressed && !mousePreviouslyPressed) m_isClicked = true;
	}
	else if (m_isHovered)
	{
		m_isHovered = false;
		setFillColor(m_color);
		setScale(m_scale);
	}
}


void SpriteButton::setup(const Texture& texture, const RenderWindow& window, const Vector2f& offsetPos)
{
	// Set Texture and Sprite for button
	m_texture = texture;
	m_sprite.setTexture(m_texture);
	m_sprite.setScale(m_scale);
	
	// Center button to origin
	FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2, bounds.height / 2);

	// Set position of button
	m_sprite.setPosition(window.getSize().x / 2.f + offsetPos.x, window.getSize().y / 2.f + offsetPos.y);

	// Set button bounds
	m_buttonBounds = m_sprite.getGlobalBounds();
}

void SpriteButton::update(Vector2i mousePos, bool mousePressed, bool mousePreviouslyPressed)
{
	m_isClicked = false;
	bool hovered = m_buttonBounds.contains(static_cast<Vector2f>(mousePos));

	if (hovered)
	{
		if (!m_isHovered)
		{
			m_isHovered = true;
			m_sprite.setColor(m_hoverColor);
			m_sprite.setScale(m_hoverScale);
		}
		if (mousePressed && !mousePreviouslyPressed)
		{
			m_isClicked = true;
		}
	}
	else if (m_isHovered)
	{
		m_isHovered = false;
		m_sprite.setColor(m_color);
		m_sprite.setScale(m_scale);
	}
}