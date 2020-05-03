
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"
#include "main.hpp"


std::array<double, 2> fixPosition(std::array<double, 2> coordinates, sf::RenderWindow &window)
{
    double pixelLength = pow(10, 4);
    coordinates[0] /= pixelLength;
    coordinates[1] /= pixelLength;
    return std::array<double, 2> {coordinates[0], window.getSize().y-coordinates[1]};
}

int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(2560, 1600), "AstroSim");

//    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // run the program as long as the window is open

    Renderer renderer(0);
    Matter planet1(pow(10,23), 10, {pow(10, 6), pow(10, 6)}, {0,0});
    Node node1(planet1, NULL, NULL);
    renderer.addMatter(node1);
    Matter planet2(pow(10,34), 11, {1.2*pow(10, 6), pow(10, 6)}, {0,0});
    Node node2(planet2, NULL, NULL);
    renderer.addMatter(node2);
    
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

//         clear the window with black color
        window.clear();

//         draw everything here...
//         window.draw(...);

        renderer.updateScene();
        Node * node = &renderer.matter;
        
        while (true)
        {
            std::array<double, 2> fixedPosition = fixPosition(node->value->position, window);
            std::cout << fixedPosition[0] << " " << fixedPosition[1] << std::endl;
            sf::CircleShape shape (node->value->radius);
            shape.setPosition(fixedPosition[0], fixedPosition[1]);
            window.draw(shape);
            if (node->next != NULL)
            {
                node = node->next;
            }
            else
            {
                break;
            }
        }
//        shape.setPosition(120, 1500);
        window.display();
    }


    return EXIT_SUCCESS;
}


//#include <SFML/Graphics.hpp>
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
//    sf::CircleShape shape(100.f);
//    shape.setFillColor(sf::Color::Green);
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        window.clear();
//        window.draw(shape);
//        window.display();
//    }
//
//    return 0;
//}
