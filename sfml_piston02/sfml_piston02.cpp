// Reciprocating piston using SFML
// August 20, 2022
// by SM

// March 11, 2025
// changes:
// update SFML to v3.0

#include <SFML/Graphics.hpp>
import std;

int main()
{
  sf::RenderWindow window{
    sf::VideoMode{{ 600, 400 }},
    "SFML works!",
    sf::Style::Titlebar | sf::Style::Close
  };
  sf::CircleShape circle{ 100.f };

  circle.setFillColor(sf::Color::Transparent);
  circle.setOutlineThickness(1);
  circle.setOutlineColor(sf::Color::Red);
  circle.setPosition({ 100, 100 });

  sf::VertexArray lines{ sf::PrimitiveType::LineStrip, 3 }; // crank shaft and piston rod

  lines[0].position = sf::Vector2f{ 200, 200 };
  lines[1].position = sf::Vector2f{ 300, 200 };
  lines[2].position = sf::Vector2f{ 450, 200 };

  sf::ConvexShape piston; // draw piston
  piston.setPointCount(4);
  piston.setPoint(0, sf::Vector2f{ 25, 10 });
  piston.setPoint(1, sf::Vector2f{ -25, 10 });
  piston.setPoint(2, sf::Vector2f{ -25, -10 });
  piston.setPoint(3, sf::Vector2f{ 25, -10 });
  piston.setFillColor(sf::Color::Yellow);
  piston.setOutlineColor(sf::Color::Red);
  piston.setOutlineThickness(3);
  piston.setPosition({ lines[2].position.x, lines[2].position.y });

  sf::Vertex hline[]  // horizontal line
  {
      sf::Vertex{sf::Vector2f{10, 200}},
      sf::Vertex{sf::Vector2f{590, 200}}
  };

  hline[0].color = sf::Color::Green;
  hline[1].color = sf::Color::Green;


  float theta{ 0.0f };
  int sleep_time{ 50 };
  while (window.isOpen())
  {
    while (const std::optional event{ window.pollEvent() })
    {
      if (event->is<sf::Event::Closed>())
      {
        window.close();
      }
      else if (const auto * keyPressed{ event->getIf<sf::Event::KeyPressed>() })
      {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Up)
        {
          sleep_time -= 5;
          sleep_time = std::max(sleep_time, 15);
        }
        else if (keyPressed->scancode == sf::Keyboard::Scancode::Down)
        {
          sleep_time += 5;
          sleep_time = std::min(sleep_time, 100);
        }
      }
    }

    window.clear();

    window.draw(circle);
    window.draw(piston);
    window.draw(lines);
    window.draw(hline, 2, sf::PrimitiveType::LineStrip);

    window.display();

    sf::sleep(sf::milliseconds(sleep_time));

    lines[1].position.x = 100 * std::cos(theta) + 200;
    lines[1].position.y = -100 * std::sin(theta) + 200;
    lines[2].position.x = std::sqrtf(
      std::powf(150.0f, 2.0f) - 
      std::powf(lines[2].position.y - lines[1].position.y, 2.0f)) + 
      lines[1].position.x;

    piston.setPosition({ lines[2].position.x, lines[2].position.y });
    theta += 0.05f;
    theta = std::fmodf(theta, 2 * std::numbers::pi);
  }
}
