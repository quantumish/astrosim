
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

#include <SFML/Graphics.hpp>

// Here is a small helper for you! Have a look.
#include "ResourcePath.hpp"
#include "main.hpp"

int main(int, char const**)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(2560, 1600), "AstroSim");

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // run the program as long as the window is open

    sf::RenderWindow * windowPointer = &window;
    Renderer renderer(0, windowPointer, pow(10, 2));
    renderer.addMatter(pow(10,1), 3, {30000, 110000}, {0,50});
    renderer.addMatter(pow(10,18), 11, {40000, 100000}, {0,0});
    int frames = 0;
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
        
        renderer.nextFrame();
        window.display();
        frames++;
    }
    return EXIT_SUCCESS;
}
