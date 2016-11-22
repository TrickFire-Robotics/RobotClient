#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;

namespace trickfire {

class DrawingUtil {
public:
	static inline Vector2f DrawGenericHeader(std::string text, Vector2f position, bool centered,
			Font& font, const Color & color, RenderWindow& window) {
		Text header;
		header.setFont(font);
		header.setCharacterSize(48);
		header.setColor(color);
		header.setStyle(Text::Italic);
		header.setString(text);
		if (centered) {
			header.setOrigin(
					Vector2f(header.getLocalBounds().width / 2,
							header.getLocalBounds().height));
		}
		header.setPosition(position);
		window.draw(header);
		return Vector2f(header.getLocalBounds().width, header.getLocalBounds().height);
	}

	static inline void DrawCenteredAxisBar(double value, Vector2f position, Vector2f dimension,
			Vector2f border, const Color & back, const Color & front, RenderWindow& window) {
		RectangleShape background(dimension);
		background.setFillColor(back);
		background.setPosition(position);
		window.draw(background);

		RectangleShape bar(
				Vector2f(dimension.x - (border.x * 2),
						-value * ((dimension.y - (border.y * 2)) / 2)));
		bar.setFillColor(front);
		bar.setPosition(
				Vector2f(position.x + border.x, position.y + (dimension.y / 2)));
		window.draw(bar);
	}
};
}
