#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "Cell.h"

int main()
{
	sf::RenderWindow window( sf::VideoMode( { 1200, 700 } ), "SFML works!" );

	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();

			// on resized event
			if ( const auto* resizedEvent = event->getIf<sf::Event::Resized>() )
			{
				// created a view from fixed visible area using FloatRect
				sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(resizedEvent->size.x, resizedEvent->size.y));
				window.setView(sf::View(visibleArea));
			}


		}

		window.clear();
		Cell testCell;

		testCell.draw(window);
		window.display();
	}
}
